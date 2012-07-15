#include "Image.h"
#include "Debug.h"
#include <iostream>
#include <GL/glu.h>
#include <GL/gl.h>
#include <fstream>

ERR_CODE Image::load(const std::string &file)
{
    name = file;
    std::string file_extension = extension(file);
	ERR_CODE ret;

    if (strieq(file_extension, "TGA"))
        ret = __loadTGA(file);
    else
        ret = INVALID_EXTENSION;

    if (ret != NONE)
        badfile = file;

	return ret;
}

ERR_CODE Image::loadTGA(const std::string &file)
{
    name = file;
    std::string file_extension = extension(file);

    if (strieq(file_extension, "TGA"))
        return __loadTGA(file);
    else
        return INVALID_EXTENSION;
}

ERR_CODE Image::__loadTGA(const std::string &filename) //TODO: load other formats
{
    ASSERT(!pixels);
    name = filename;
    FILE *file = file_open("Images/" + filename, "rb");

    if (!file)
        return FILE_NOT_FOUND;

    GLubyte uncompressed_top_header[12] = { 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    GLubyte compressed_top_header[12] = { 0, 0, 10, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    GLubyte real_top_header[12];
    fread(real_top_header, 1, 12, file);

    if (memcmp(real_top_header, uncompressed_top_header, 12) == 0)
        return __loadUncompressedTGA(file);

    else if (memcmp(real_top_header, compressed_top_header, 12) == 0)
        return __loadCompressedTGA(file);

    else
    {
        fclose(file);
        return UNSUPPORTED_FILE_TYPE;
    }
}

ERR_CODE Image::__loadUncompressedTGA(FILE *file)
{
    GLubyte header[6];
    fread(header, 1, 6, file);

    if (header[4] != 24 && header[4] != 32)
    {
        fclose(file);
        return BAD_BYTES_PER_PIXEL;
    }

    bpp = header[4] / 8;
    width = header[1] * 256 + header[0];
    height = header[3] * 256 + header[2];

    int filesize = width * height * bpp;
    pixels = new GLubyte[filesize];
    int bytesRead = fread(pixels, 1, filesize, file);

    if (bytesRead != filesize)
    {
        fclose(file);
        return CORRUPTED_FILE;
    }

    GLubyte temp;

    for (int c = 0; c < filesize; c += bpp)
    {
        temp = pixels[c];
        pixels[c] = pixels[c + 2];
        pixels[c + 2] = temp;
    }

    fclose(file);
    return NONE;
}

ERR_CODE Image::__loadCompressedTGA(FILE *file)
{
    GLubyte header[6];
    fread(header, 1, 6, file);

    if (header[4] != 24 && header[4] != 32)
    {
        fclose(file);
        return BAD_BYTES_PER_PIXEL;
    }

    bpp = header[4] / 8;
    width = header[1] * 256 + header[0];
    height = header[3] * 256 + header[2];

    int filesize = width * height * bpp;
    pixels = new GLubyte[filesize];
    GLubyte *color_buffer = new GLubyte[bpp];
    GLuint current_pixel = 0, current_byte = 0, pixel_count = width * height;

    do
    {
        GLubyte chunk_header;

        if (fread(&chunk_header, sizeof(GLubyte), 1, file) == 0)
        {
            fclose(file);
            return CORRUPTED_FILE;
        }

        if (chunk_header < 128)
        {
            ++chunk_header;

            for (int counter = 0; counter < chunk_header; counter++)
            {
                if (fread(color_buffer, 1, bpp, file) != bpp)
                {
                    fclose(file);

                    if (color_buffer)
                        delete[] color_buffer;

                    return CORRUPTED_FILE;
                }

                pixels[current_byte] = color_buffer[2];
                pixels[current_byte + 1] = color_buffer[1];
                pixels[current_byte + 2] = color_buffer[0];

                if (bpp == 4)
                    pixels[current_byte + 3] = color_buffer[3];

                current_byte += bpp;
                ++current_pixel;

                if (current_pixel > pixel_count)
                {
                    fclose(file);
                    delete[] color_buffer;
                    return CORRUPTED_FILE;
                }
            }
        }

        else
        {
            chunk_header -= 127;

            if (fread(color_buffer, 1, bpp, file) != bpp)
            {
                fclose(file);
                delete[] color_buffer;
                return CORRUPTED_FILE;
            }

            for (int counter = 0; counter < chunk_header; counter++)
            {
                pixels[current_byte] = color_buffer[2];
                pixels[current_byte + 1] = color_buffer[1];
                pixels[current_byte + 2] = color_buffer[0];

                if (bpp == 4)
                    pixels[current_byte + 3] = color_buffer[3];

                current_byte += bpp;
                ++current_pixel;

                if (current_pixel > pixel_count)
                {
                    fclose(file);
                    delete[] color_buffer;
                    return CORRUPTED_FILE;
                }
            }
        }
    }
    while (current_pixel < pixel_count);
    fclose(file);
    delete[] color_buffer;
    return NONE;
}

ERR_CODE Image::loadRAW(const std::string &file, const GLsizei w, const GLsizei h)
{
    name = file;
    std::string file_extension = extension(file);

    if (strieq(file_extension, "RAW"))
        return __loadRAW(file, w, h);
    else
        return INVALID_EXTENSION;
}

ERR_CODE Image::__loadRAW(const std::string &file, const GLsizei w, const GLsizei h)
{
    ASSERT(!pixels);
    name = file;
    width = w;
    height = h;
    bpp = 3;

    FILE *in = file_open("Images/" + file, "rb");

    if (!in)
        return FILE_NOT_FOUND;

    pixels = new GLubyte[w * h * 3];

    int bytesRead = fread(pixels, 1, w * h * 3, in);
    if (bytesRead != w * h * 3)
        return CORRUPTED_FILE;
    fclose(in);
    return NONE;
}
