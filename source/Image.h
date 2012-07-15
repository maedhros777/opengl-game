#ifndef IMAGE_H
#define IMAGE_H

#include <GL/gl.h>
#include <vector>
#include <string>
#include <cstring>
#include "Error.h"
#include "Utils.h"

class Image
{
    friend class TextureManager;
private:
    std::string name;
    GLsizei width, height;
    GLubyte bpp;
    GLubyte *pixels;
    ERR_RET __loadTGA(const std::string &);
    ERR_RET __loadCompressedTGA(FILE *);
    ERR_RET __loadUncompressedTGA(FILE *);
    ERR_RET __loadRAW(const std::string &, const GLsizei, const GLsizei);
public:
    Image() : width(0), height(0), bpp(0), pixels(0)
    {
    }
    ~Image()
    {
        delete[] pixels;
    }
    ERR_RET load(const std::string &);
    ERR_RET loadTGA(const std::string &);
    ERR_RET loadRAW(const std::string &, const GLsizei, const GLsizei);
    GLsizei getWidth() const
    {
        return width;
    }
    GLsizei getHeight() const
    {
        return height;
    }
    std::string getName() const
    {
        return name;
    }
    void setName(const std::string &_name)
    {
        name = _name;
    }
    GLsizei size() const
    {
        return width * height * bpp;
    }
    GLubyte BPP() const
    {
        return bpp;
    }
};

#endif
