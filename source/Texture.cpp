#include "Texture.h"
#include "Image.h"
#include <GL/glu.h>

Texture &Texture::operator=(const Texture &tex)
{
    if (isHandle)
        textureManager.release(name);
    id = tex.id;
    name = tex.name;
    isHandle = true;
    textureManager.addRef(name);
    return *this;
}

Texture TextureManager::load(const std::string &name, GLubyte bpp, GLsizei width, GLsizei height, GLubyte *pixels, bool repeat)
{
    Texture tex;
    tex.name = name;
    glGenTextures(1, &tex.id);
    glBindTexture(GL_TEXTURE_2D, tex.id);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, repeat ? GL_REPEAT : GL_CLAMP);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, repeat ? GL_REPEAT : GL_CLAMP);
    gluBuild2DMipmaps(GL_TEXTURE_2D, bpp, width, height, bpp == 4 ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, pixels);
    textures[tex.name].set(tex, 0);
    return tex;
}

std::pair<Texture, ERR_CODE> CHECK TextureManager::load(const std::string &filename, bool repeat)
{
    std::map<std::string, TextureHandle>::iterator tex_pair = textures.find(filename);

    if (tex_pair != textures.end())
        return std::make_pair(tex_pair->second.tex, NONE);

    Image img;
    ERR_CODE ret = img.load(filename);

    if (ret != NONE)
        return std::make_pair(Texture(), ret);

    return std::make_pair(load(img, repeat), NONE);
}

void TextureManager::release(const std::string &name)
{
    if (--textures[name].refs <= 0)
    {
        glDeleteTextures(1, &textures[name].tex.id);
        textures.erase(name);
    }
}
