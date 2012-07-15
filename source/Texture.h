#ifndef TEXTURE_H
#define TEXTURE_H
#include "Image.h"
#include "Debug.h"
#include "Singleton.h"
#include <string>
#include <map>
#include <vector>
#include <GL/gl.h>

class Texture
{
    friend class TextureHandle;
    friend class TextureManager;
private:
    GLuint id;
    std::string name;
    bool isHandle;

    void __assign(const Texture &tex)
    {
        id = tex.id;
    }

public:
    Texture() : isHandle(false)  { }
    Texture(const Texture &);
    inline ~Texture();
    Texture &operator=(const Texture &);

	std::string get_name() const
	{
		return name;
	}
};

struct TextureHandle
{
    Texture tex;
    int refs;

    TextureHandle() : refs(0) { }

    TextureHandle(const TextureHandle &th) : refs(th.refs)
    {
        tex.__assign(th.tex);
    }

    TextureHandle &operator=(const TextureHandle &th)
    {
        tex.__assign(th.tex);
        refs = th.refs;
        return *this;
    }

    void set(const Texture &_tex, int _refs)
    {
        tex.__assign(_tex);
        refs = _refs;
    }
};

class TextureManager : public Singleton<TextureManager>
{
private:
    std::map<std::string, TextureHandle> textures;
public:
    TextureManager()
    {
        glEnable(GL_TEXTURE_2D);
    }

    ~TextureManager()
    {
        for (std::map<std::string, TextureHandle>::iterator it = textures.begin(); it != textures.end(); ++it)
            glDeleteTextures(1, &(it->second.tex.id));
    }

    Texture load(const std::string &name, GLubyte bpp, GLsizei width, GLsizei height, GLubyte *pixels, bool repeat = true);
    std::pair<Texture, ERR_CODE> CHECK load(const std::string &filename, bool repeat = true);
    void release(const std::string &name);

    Texture load(const Image &img, bool repeat = true)
    {
        return load(img.name, img.bpp, img.width, img.height, img.pixels, repeat);
    }

    void addRef(const std::string &name)
    {
        ++textures[name].refs;
    }

    unsigned int size()
    {
        return textures.size();
    }

    void bind(const Texture &tex) const
    {
        glBindTexture(GL_TEXTURE_2D, tex.id);
    }
};

#define textureManager TextureManager::getInstance()
#define textureManagerPtr TextureManager::getInstancePtr()

inline Texture::Texture(const Texture &tex) : id(tex.id), name(tex.name), isHandle(true)
{
    textureManager.addRef(name);
}

inline Texture::~Texture()
{
    if (isHandle && TextureManager::valid())
        textureManager.release(name);
}

#endif
