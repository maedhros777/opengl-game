#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <cstdio>
#include <sstream>
#include "Error.h"

#define TEX_LOAD(var, filename, err_handling) \
    dummy_tex_pair.first = Texture(); \
    dummy_tex_pair = textureManager.load(filename); \
    if (dummy_tex_pair.second != NONE) \
        err_handling; \
    var = dummy_tex_pair.first
#define TEX_ERR dummy_tex_pair.second
#define TEX_ERR_STR errString(dummy_tex_pair.second)

extern std::string PATH;
class Texture;
extern std::pair<Texture, ERR_CODE> dummy_tex_pair;

inline void set_path(const std::string &path)
{
    PATH = path;
}

inline std::string extension(const std::string &file)
{
    return file.substr(file.find_last_of('.') + 1);
}

inline FILE *file_open(const std::string &img, const char *mode)
{
    return fopen((PATH + "Data/" + img).c_str(), mode);
}

inline std::string getpath(const std::string &filename)
{
    unsigned int pos = filename.rfind('/');
    return (pos != std::string::npos ? filename.substr(0, pos + 1) : "");
}

class Quaternion;
void disp(const Quaternion &q);
void disp_mat(float mat[16]);
void die(const std::string &msg);
bool strieq(const std::string &str1, const std::string &str2);
std::string itostr(int num);
void disp_mat_mult(float mat[16], float x, float y, float z);

#endif
