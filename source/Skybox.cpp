#include "Skybox.h"
#include "Image.h"
#include "Texture.h"

void Skybox::init(const Image &front, const Image &left, const Image &back, const Image &right, const Image &up, const Image &down)
{
    textures[0] = textureManager.load(front, false);
    textures[1] = textureManager.load(left, false);
    textures[2] = textureManager.load(back, false);
    textures[3] = textureManager.load(right, false);
    textures[4] = textureManager.load(up, false);
    textures[5] = textureManager.load(down, false);
}

void Skybox::render(const Vector3f &size) const
{
    glPushMatrix();
    glScalef(size.getX(), size.getY(), size.getZ());

    glPushAttrib(GL_ENABLE_BIT);
    glEnable(GL_TEXTURE_2D);
    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);
    glColor4f(1.0, 1.0, 1.0, 1.0);

    //front
    textureManager.bind(textures[0]);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0, 0.0);
    glVertex3f(0.5f, -0.5f, -0.5f);
    glTexCoord2f(1.0, 0.0);
    glVertex3f(-0.5f, -0.5f, -0.5f);
    glTexCoord2f(1.0, 1.0);
    glVertex3f(-0.5f,  0.5f, -0.5f);
    glTexCoord2f(0.0, 1.0);
    glVertex3f(0.5f,  0.5f, -0.5f);
    glEnd();

    //left
    textureManager.bind(textures[1]);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0, 0.0);
    glVertex3f(0.5f, -0.5f,  0.5f);
    glTexCoord2f(1.0, 0.0);
    glVertex3f(0.5f, -0.5f, -0.5f);
    glTexCoord2f(1.0, 1.0);
    glVertex3f(0.5f,  0.5f, -0.5f);
    glTexCoord2f(0.0, 1.0);
    glVertex3f(0.5f,  0.5f,  0.5f);
    glEnd();

    //back
    textureManager.bind(textures[2]);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0, 0.0);
    glVertex3f(-0.5f, -0.5f,  0.5f);
    glTexCoord2f(1.0, 0.0);
    glVertex3f(0.5f, -0.5f,  0.5f);
    glTexCoord2f(1.0, 1.0);
    glVertex3f(0.5f,  0.5f,  0.5f);
    glTexCoord2f(0.0, 1.0);
    glVertex3f(-0.5f,  0.5f,  0.5f);

    glEnd();

    //right
    textureManager.bind(textures[3]);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0, 0.0);
    glVertex3f(-0.5f, -0.5f, -0.5f);
    glTexCoord2f(1.0, 0.0);
    glVertex3f(-0.5f, -0.5f,  0.5f);
    glTexCoord2f(1.0, 1.0);
    glVertex3f(-0.5f,  0.5f,  0.5f);
    glTexCoord2f(0.0, 1.0);
    glVertex3f(-0.5f,  0.5f, -0.5f);
    glEnd();

    //up
    textureManager.bind(textures[4]);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0, 1.0);
    glVertex3f(-0.5f,  0.5f, -0.5f);
    glTexCoord2f(0.0, 0.0);
    glVertex3f(-0.5f,  0.5f,  0.5f);
    glTexCoord2f(1.0, 0.0);
    glVertex3f(0.5f,  0.5f,  0.5f);
    glTexCoord2f(1.0, 1.0);
    glVertex3f(0.5f,  0.5f, -0.5f);
    glEnd();

    //down
    textureManager.bind(textures[5]);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0, 0.0);
    glVertex3f(-0.5f, -0.5f, -0.5f);
    glTexCoord2f(0.0, 1.0);
    glVertex3f(-0.5f, -0.5f,  0.5f);
    glTexCoord2f(1.0, 1.0);
    glVertex3f(0.5f, -0.5f,  0.5f);
    glTexCoord2f(1.0, 0.0);
    glVertex3f(0.5f, -0.5f, -0.5f);
    glEnd();

    glPopAttrib();
    glPopMatrix();
}
