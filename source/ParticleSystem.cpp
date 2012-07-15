#include "ParticleSystem.h"

void ParticleSystem::render() const
{
    std::vector<Particle *> particle_vec;
    int i;

    for (i = 0; i < num_particles; ++i)
        particle_vec.push_back(particles + i);

    std::sort(particle_vec.begin(), particle_vec.end(), back);

    for (i = 0; i < num_particles; ++i)
    {
        showCol3f(color);
        float x = particle_vec[i]->pos.getX(), y = particle_vec[i]->pos.getY(), z = particle_vec[i]->pos.getZ();

        glBegin(GL_QUADS); //TODO: use vertex array
        glTexCoord2f(0.0, 0.0);
        glVertex3f(x - particle_offset, y - particle_offset, z);
        glTexCoord2f(1.0, 0.0);
        glVertex3f(x + particle_offset, y - particle_offset, z);
        glTexCoord2f(1.0, 1.0);
        glVertex3f(x + particle_offset, y + particle_offset, z);
        glTexCoord2f(0.0, 1.0);
        glVertex3f(x - particle_offset, y + particle_offset, z);
        glEnd();
    }
}

void Fountain::init(Particle &particle)
{
    particle.pos.set(0.0, 0.0, 0.0);
    particle.velocity.set(randInRange(x_var), 0, randInRange(z_var));
    particle.velocity.setY(absRandInRange(y_max - y_min) + y_min);
    particle.lifetime = absRandInRange(max_lifetime);
}

void Fountain::update()
{
    for (int i = 0; i < num_particles; ++i)
    {
        --particles[i].lifetime;
        particles[i].velocity.setY(particles[i].velocity.getY() - gravity);
        particles[i].pos += particles[i].velocity;

        if (particles[i].pos.getY() < 0)
            particles[i].lifetime = 0;

        if (particles[i].lifetime == 0)
            init(particles[i]);
    }
}

void Snow::init(Particle &particle, bool immediate)
{
    particle.pos = Point3f(randInRange(1.0), immediate ? absRandInRange(1.0) : 1.0, randInRange(1.0));
    particle.velocity = Vector3f(0.0, -(absRandInRange(max_v - min_v) + min_v), 0.0);
}

void Snow::update()
{
    for (int i = 0; i < num_particles; ++i)
    {
        particles[i].pos += particles[i].velocity;

        if (particles[i].pos.getY() < 0.0)
            init(particles[i]);
    }
}

void Fireworks::init()
{
    _dead = false;
	exploded = false;
	rocket_velocity = initial_rocket_v;
	rocket_lifetime = max_rocket_lifetime;
    rocket_pos.set(0.0, 0.0, 0.0);

    for (int i = 0; i < num_particles; ++i)
        particles[i].pos = randVec(Vector3f(radius, radius, radius));
}

void Fireworks::update()
{
    if (_dead)
        return;
    int i;

    if (!exploded)
    {
        if (--rocket_lifetime == 0)
        {
            for (i = 0; i < num_particles; ++i)
            {
                particles[i].velocity = ((particles[i].pos - rocket_pos).normal() * speed) - gravity;
                particles[i].pos += particles[i].velocity;
                particles[i].lifetime = absRandInRange(max_lifetime);
                particles[i].time_alive = 0;
            }

            exploded = true;
        }

        else
        {
            rocket_velocity.setY(rocket_velocity.getY() - gravity);
            rocket_pos += rocket_velocity;

            for (i = 0; i < num_particles; ++i)
                particles[i].pos += rocket_velocity;
        }
    }

    else
    {
        _dead = true;

        for (i = 0; i < num_particles; ++i)
        {
            if (particles[i].lifetime)
            {
                --particles[i].lifetime;
                ++particles[i].time_alive;
                _dead = false;
            }

            particles[i].velocity.setY(particles[i].velocity.getY() - gravity);
            particles[i].pos += particles[i].velocity;

            if (particles[i].pos.getY() < 0)
                particles[i].lifetime = 0;
        }
    }
}

void Fireworks::render() const
{
    if (_dead)
        return;

    if (exploded)
    {
        std::vector<Particle *> particle_vec;
        int i;

        for (i = 0; i < num_particles; ++i)
            particle_vec.push_back(particles + i);

        std::sort(particle_vec.begin(), particle_vec.end(), back);

        for (int i = 0; i < num_particles; ++i)
        {
            glColor4f(color.getRed(), color.getGreen(), color.getBlue(), (float) particle_vec[i]->lifetime / (particle_vec[i]->lifetime +
                particle_vec[i]->time_alive));
            float x = particle_vec[i]->pos.getX(), y = particle_vec[i]->pos.getY(), z = particle_vec[i]->pos.getZ();

            glBegin(GL_QUADS);
            glTexCoord2f(0.0, 0.0);
            glVertex3f(x - particle_offset, y - particle_offset, z);
            glTexCoord2f(1.0, 0.0);
            glVertex3f(x + particle_offset, y - particle_offset, z);
            glTexCoord2f(1.0, 1.0);
            glVertex3f(x + particle_offset, y + particle_offset, z);
            glTexCoord2f(0.0, 1.0);
            glVertex3f(x - particle_offset, y + particle_offset, z);
            glEnd();
        }
    }

    else
    {
        showCol3f(color);
        float x = rocket_pos.getX(), y = rocket_pos.getY(), z = rocket_pos.getZ();
        float temp_offset = particle_offset * 3;

        glBegin(GL_QUADS);
        glTexCoord2f(0.0, 0.0);
        glVertex3f(x - temp_offset, y - temp_offset, z);
        glTexCoord2f(1.0, 0.0);
        glVertex3f(x + temp_offset, y - temp_offset, z);
        glTexCoord2f(1.0, 1.0);
        glVertex3f(x + temp_offset, y + temp_offset, z);
        glTexCoord2f(0.0, 1.0);
        glVertex3f(x - temp_offset, y + temp_offset, z);
        glEnd();
    }
}
