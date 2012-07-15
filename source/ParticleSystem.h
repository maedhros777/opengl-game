#ifndef PARTICLESYSTEM_H
#define PARTICLESYSTEM_H
#include "Point.h"
#include "Vector.h"
#include "Color.h"
#include "Debug.h"
#include <cstdlib>
#include <vector>
#include <algorithm>

const float PARTICLE_OFFSET = 0.03;
const float GRAVITY = 0.013;
const int NUM_PARTICLES = 1000;

const int FOUNTAIN_MAX_LIFETIME = 60;
const float FOUNTAIN_X_VAR = 0.04;
const float FOUNTAIN_Y_MAX = 0.45;
const float FOUNTAIN_Y_MIN = 0.35;
const float FOUNTAIN_Z_VAR = 0.04;
const Color3f FOUNTAIN_COLOR(0.0, 0.8, 1.0);

const int FIREWORKS_MAX_LIFETIME = 30;
const int FIREWORKS_ROCKET_LIFETIME = 35;
const float FIREWORKS_ROCKET_X = 0.01;
const float FIREWORKS_ROCKET_Y = 0.34;
const float FIREWORKS_ROCKET_Z = 0.0;
const float FIREWORKS_SPEED = 0.1;
const float FIREWORKS_GRAVITY = 0.006;
const float FIREWORKS_RADIUS = 0.05;
const Color3f FIREWORKS_COLOR(1.0, 0.0, 0.0);

const float SNOW_MAX_VELOCITY = 0.009;
const float SNOW_MIN_VELOCITY = 0.003;
const float SNOW_PARTICLE_OFFSET = 0.01;
const Color3f SNOW_COLOR(1.0, 1.0, 1.0);

struct Particle
{
    Point3f pos;
    Vector3f velocity;
    unsigned int lifetime, time_alive;
};

class ParticleSystem
{
protected:
    float particle_offset, gravity;
    Particle *particles;
    int num_particles;
    Color3f color;
public:
    ParticleSystem(int num = NUM_PARTICLES, const Color3f &col = RED, float _gravity = GRAVITY, float offset = PARTICLE_OFFSET) :
        particle_offset(offset), gravity(_gravity), num_particles(num), color(col)
    {
        particles = new Particle[num_particles];
    }

    virtual ~ParticleSystem()
    {
        delete[] particles;
    }

    virtual void update() = 0;
    virtual void render() const;

    float getParticleOffset() const
    {
        return particle_offset;
    }

    void setParticleOffset(float offset)
    {
        ASSERT(offset > 0);
        particle_offset = offset;
    }

    float getGravity() const
    {
        return gravity;
    }

    void setGravity(float g)
    {
        ASSERT(g > 0);
        gravity = g;
    }

    int getNumParticles() const
    {
        return num_particles;
    }
};

class Fountain : public ParticleSystem
{
private:
    int max_lifetime;
    float x_var, y_max, y_min, z_var;

    void init(Particle &particle);
public:
    Fountain(int num = NUM_PARTICLES, int lifetime = FOUNTAIN_MAX_LIFETIME, float _x_var = FOUNTAIN_X_VAR, float _y_max = FOUNTAIN_Y_MAX,
        float _y_min = FOUNTAIN_Y_MIN, float _z_var = FOUNTAIN_Z_VAR, const Color3f &col = FOUNTAIN_COLOR) : ParticleSystem(num, col),
        max_lifetime(lifetime), x_var(_x_var), y_max(_y_max), y_min(_y_min), z_var(_z_var)
    {
        for (int i = 0; i < num_particles; ++i)
            init(particles[i]);
    }

    void update();

    int getMaxLifetime() const
    {
        return max_lifetime;
    }

    void setMaxLifetime(int max)
    {
        ASSERT(max > 0);
        max_lifetime = max;
    }

    float getXVar() const
    {
        return x_var;
    }

    void setXVar(float var)
    {
        ASSERT(var > 0);
        x_var = var;
    }

    float getZVar() const
    {
        return z_var;
    }

    void setZVar(float var)
    {
        ASSERT(var > 0);
        z_var = var;
    }

    float getYMin() const
    {
        return y_min;
    }

    void setYMin(float min)
    {
        y_min = min;
    }

    float getYMax() const
    {
        return y_max;
    }

    void setYMax(float max)
    {
        y_max = max;
    }

    Color3f getColor() const
    {
        return color;
    }

    void setColor(const Color3f col)
    {
        color = col;
    }
};

class Snow : public ParticleSystem
{
private:
    float max_v, min_v;
    void init(Particle &particle, bool immediate = false);
public:
    Snow(int num = NUM_PARTICLES, const Color3f &col = SNOW_COLOR, float offset = SNOW_PARTICLE_OFFSET,
        float grav = GRAVITY, float max_velocity = SNOW_MAX_VELOCITY, float min_velocity = SNOW_MIN_VELOCITY, bool immediate = false) :
        ParticleSystem(num, col, grav, offset), max_v(SNOW_MAX_VELOCITY), min_v(min_velocity)
    {
        init(immediate);
    }

	void init(bool immediate = false)
	{
		for (int i = 0; i < num_particles; ++i)
			init(particles[i], immediate);
	}

    void update();
};

class Fireworks : public ParticleSystem
{
private:
    int max_lifetime, rocket_lifetime, max_rocket_lifetime;
    Vector3f rocket_velocity, initial_rocket_v;
    Point3f rocket_pos;
    bool exploded, _dead;
    float radius, speed;
public:
    Fireworks(int num = NUM_PARTICLES, int max_life = FIREWORKS_MAX_LIFETIME, int rocket_life = FIREWORKS_ROCKET_LIFETIME, Vector3f rocket_v =
            Vector3f(FIREWORKS_ROCKET_X, FIREWORKS_ROCKET_Y, FIREWORKS_ROCKET_Z), const Color3f &col = FIREWORKS_COLOR,
            float grav = FIREWORKS_GRAVITY, float _speed = FIREWORKS_SPEED, float rad = FIREWORKS_RADIUS) : ParticleSystem(num, col, grav),
            max_lifetime(max_life), max_rocket_lifetime(rocket_life), initial_rocket_v(rocket_v), radius(rad), speed(_speed)
    {
        init();
    }

    void init();
    void update();
    void render() const;

    int getMaxLifetime() const
    {
        return max_lifetime;
    }

    void setMaxLifetime(int life)
    {
        ASSERT(life > 0);
        max_lifetime = life;
    }

    bool dead() const
    {
        return _dead;
    }
};

inline float frand()
{
    return (float)rand() / RAND_MAX;
}

inline float randInRange(float range)
{
    return frand() * range * 2 - range;
}

inline float absRandInRange(float range)
{
    return frand() * range;
}

inline Vector3f randVec(const Vector3f &vec)
{
    return Vector3f(randInRange(vec.getX()), randInRange(vec.getY()), randInRange(vec.getZ()));
}

inline Vector3f absRandVec(const Vector3f &vec)
{
    return Vector3f(absRandInRange(vec.getX()), absRandInRange(vec.getY()), absRandInRange(vec.getZ()));
}

inline bool back(Particle *first, Particle *second)
{
    return first->pos.getZ() < second->pos.getZ();
}

#endif
