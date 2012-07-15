#ifndef COLOR_H
#define COLOR_H
#include "Debug.h"

#include <GL/gl.h>
#define BLACK Color3f(0.0f, 0.0f, 0.0f)
#define WHITE Color3f(1.0f, 1.0f, 1.0f)
#define RED Color3f(1.0f, 0.0f, 0.0f)
#define GREEN Color3f(0.0f, 1.0f, 0.0f)
#define BLUE Color3f(0.0f, 0.0f, 1.0f)

template <class T>
class Color3
{
private:
    T red, green, blue;
public:
    Color3(T r = 0.0, T g = 0.0, T b = 0.0) : red(r), green(g), blue(b)
    {
    }

    Color3(const Color3<T> &c)
    {
        set(c);
    }

    void set(T r, T g, T b)
    {
        ASSERT(r >= 0.0 && r <= 1.0);
        red = r;
        ASSERT(g >= 0.0 && g <= 1.0);
        green = g;
        ASSERT(b >= 0.0 && b <= 1.0);
        blue = b;
    }

    void set(const Color3<T> &col)
    {
        set(col.red, col.green, col.blue);
    }
    void setRed(T r)
    {
        if (r >= 0.0 && r <= 1.0)
            red = r;
    }
    void setGreen(T g)
    {
        if (g >= 0.0 && g <= 1.0)
            green = g;
    }
    void setBlue(T b)
    {
        if (b >= 0.0 && b <= 1.0)
            blue = b;
    }
    T getRed() const
    {
        return red;
    }
    T getGreen() const
    {
        return green;
    }
    T getBlue() const
    {
        return blue;
    }
    bool operator==(const Color3<T> &c) const
    {
        return (red == c.red && green == c.green && blue == c.blue);
    }
    bool operator!=(const Color3<T> &c) const
    {
        return !(*this == c);
    }
};

template <>
class Color3<GLubyte>
{
private:
    GLubyte red, green, blue;
public:
    Color3(GLubyte r = 0, GLubyte g = 0, GLubyte b = 0)
    {
        set(r, g, b);
    }
    Color3(const Color3<GLubyte> &c)
    {
        set(c);
    }
    void set(GLubyte r, GLubyte g, GLubyte b)
    {
        red = r;
        green = g;
        blue = b;
    }
    void set(const Color3<GLubyte> &col)
    {
        set(col.red, col.green, col.blue);
    }
    void setRed(GLubyte r)
    {
        red = r;
    }
    void setGreen(GLubyte g)
    {
        green = g;
    }
    void setBlue(GLubyte b)
    {
        blue = b;
    }
    GLubyte getRed() const
    {
        return red;
    }
    GLubyte getGreen() const
    {
        return green;
    }
    GLubyte getBlue() const
    {
        return blue;
    }
    bool operator==(const Color3<GLubyte> &) const;
    bool operator!=(const Color3<GLubyte> &c) const
    {
        return !(*this == c);
    }
};

typedef Color3<GLfloat> Color3f;
typedef Color3<GLdouble> Color3d;
typedef Color3<GLubyte> Color3ub;


inline void showCol3f(const Color3f &col)
{
    glColor3f(col.getRed(), col.getGreen(), col.getBlue());
}

inline void showCol3d(const Color3d &col)
{
    glColor3d(col.getRed(), col.getGreen(), col.getBlue());
}

inline void showCol3ub(const Color3ub &col)
{
    glColor3ub(col.getRed(), col.getGreen(), col.getBlue());
}

template <class T>
class Color4
{
    friend void setMatCol(GLenum, const Color4<GLfloat> &);
private:
    T red, green, blue, alpha;
public:
    Color4(T r = 0.0, T g = 0.0, T b = 0.0, T a = 1.0)
    {
        set(r, g, b, a);
    }
    Color4(const Color4<T> &c)
    {
        set(c);
    }
    void set(T r, T g, T b, T a)
    {
        ASSERT(r >= 0.0 && r <= 1.0);
        red = r;
        ASSERT(g >= 0.0 && g <= 1.0);
        green = g;
        ASSERT(b >= 0.0 && b <= 1.0);
        blue = b;
        ASSERT(a >= 0.0 && a <= 1.0);
        alpha = a;
    }
    void set(const Color4<T> &col)
    {
        set(col.red, col.green, col.blue, col.alpha);
    }
    void setRed(T r)
    {
        red = r;
    }
    void setGreen(T g)
    {
        green = g;
    }
    void setBlue(T b)
    {
        blue = b;
    }
    void setAlpha(T a)
    {
        alpha = a;
    }
    T getRed() const
    {
        return red;
    }
    T getGreen() const
    {
        return green;
    }
    T getBlue() const
    {
        return blue;
    }
    T getAlpha() const
    {
        return alpha;
    }
    bool operator==(const Color4<T> &c) const
    {
        return (red == c.red && green == c.green && blue == c.blue && alpha == c.alpha);
    }
    bool operator!=(const Color4<T> &c) const
    {
        return !(*this == c);
    }

    operator Color3<T>() const
    {
        return Color3<T>(red, green, blue);
    }
};

template <>
class Color4<GLubyte>
{
private:
    GLubyte red, green, blue, alpha;
public:
    Color4(GLubyte r = 0, GLubyte g = 0, GLubyte b = 0, GLubyte a = 255)
    {
        set(r, g, b, a);
    }
    Color4(const Color4<GLubyte> &c)
    {
        set(c);
    }
    void set(GLubyte, GLubyte, GLubyte, GLubyte);
    void set(const Color4<GLubyte> &);
    void setRed(GLubyte r)
    {
        red = r;
    }
    void setGreen(GLubyte g)
    {
        green = g;
    }
    void setBlue(GLubyte b)
    {
        blue = b;
    }
    void setAlpha(GLubyte a)
    {
        alpha = a;
    }
    GLubyte getRed() const
    {
        return red;
    }
    GLubyte getGreen() const
    {
        return green;
    }
    GLubyte getBlue() const
    {
        return blue;
    }
    GLubyte getAlpha() const
    {
        return alpha;
    }
    bool operator==(const Color4<GLubyte> &) const;
    bool operator!=(const Color4<GLubyte> &c) const
    {
        return !(*this == c);
    }
};

typedef Color4<GLfloat> Color4f;
typedef Color4<GLdouble> Color4d;
typedef Color4<GLubyte> Color4ub;

inline void showCol4f(const Color4f &col)
{
    glColor4f(col.getRed(), col.getGreen(), col.getBlue(), col.getAlpha());
}

inline void showCol4d(const Color4d &col)
{
    glColor4d(col.getRed(), col.getGreen(), col.getBlue(), col.getAlpha());
}

inline void showCol4ub(const Color4ub &col)
{
    glColor4ub(col.getRed(), col.getGreen(), col.getBlue(), col.getAlpha());
}

#endif
