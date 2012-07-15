#ifndef VECTOR_H
#define VECTOR_H
#include <iostream>
#include <GL/gl.h>
#include <cmath>
#include "Debug.h"

template <class T>
class Point2;

template <class T>
class Vector2
{
    inline friend Vector2<T> operator*(T scalar, const Vector2<T> &vec)
	{
	    return vec * scalar;
	}
    
    friend inline std::ostream &operator<<(std::ostream &stream, const Vector2<T> &vec)
	{
	    return (stream << "(" << vec.x << ", " << vec.y << ")");
	}
private:
    T x, y;
public:
    Vector2(T _x = 0, T _y = 0) : x(_x), y(_y)
	{
	}

    Vector2(const Vector2<T> &v) : x(v.x), y(v.y)
	{
	}

    void set(const Vector2<T> &v)
	{
	    set(v.x, v.y);
	}

    void set(T _x, T _y)
	{
	    x = _x;
	    y = _y;
	}

    Vector2<T> operator*(T scalar) const
	{
	    return Vector2<T>(x * scalar, y * scalar);
	}

    Vector2<T> &operator*=(T scalar)
	{
	    return (*this = *this * scalar);
	}

    T operator*(const Vector2<T> &v) const
	{
	    return dot(v);
	}

    Vector2<T> &operator*=(const Vector2<T> &v)
	{
	    return (*this = dot(v));
	}

    Vector2<T> operator/(T scalar) const
	{
	    return *this * (1.0 / scalar);
	}

    Vector2<T> operator+(const Vector2<T> &v) const
	{
	    return Vector2<T>(x + v.x, y + v.y);
	}

    Point2<T> operator+(const Point2<T> &p) const
	{
	    return Point2<T>(p.x + x, p.y + y);
	}

    Vector2<T> operator+=(const Vector2<T> &v)
	{
	    return (*this = *this + v);
	}

    Vector2<T> operator-(const Vector2<T> &v) const
	{
	    return Vector2<T>(x - v.x, y - v.y);
	}

    Vector2<T> operator-=(const Vector2<T> &v)
	{
	    return (*this = *this - v);
	}

    Vector2<T> operator-() const
	{
	    return Vector2<T>(-x, -y);
	}

    T dot(const Vector2<T> &v) const;
    T length() const;
    Vector2<T> normal() const;

    void normalize()
	{
	    *this = normal();
	}

    Vector2<T> &operator=(const Vector2<T> &v)
	{
	    set(v);
	    return *this;
	}

    bool operator==(const Vector2<T> &v) const
	{
	    return (x == v.x && y == v.y);
	}

    bool operator!=(const Vector2<T> &v) const
	{
	    return !(*this == v);
	}

    bool operator!() const
	{
	    return (x == 0 && y == 0);
	}
    
    T getX() const
	{
	    return x;
	}

    T getY() const
	{
	    return y;
	}

    void setX(T val)
	{
	    x = val;
	}

    void setY(T val)
	{
	    y = val;
	}
    
    operator Point2<T>() const
	{
	    return Point2<T>(x, y);
	}
};

typedef Vector2<GLfloat> Vector2f;
typedef Vector2<GLint> Vector2i;
typedef Vector2<GLdouble> Vector2d;
typedef Vector2<GLshort> Vector2s;

template <class T>
class Point3;

template <class T>
class Vector3
{
    inline friend Vector3<T> operator*(T scalar, const Vector3<T> &vec)
	{
	    return vec * scalar;
	}
    
    friend inline std::ostream &operator<<(std::ostream &stream, const Vector3<T> &vec)
	{
	    return (stream << "(" << vec.x << ", " << vec.y << ", " << vec.z << ")");
	}
private:
    T x, y, z;
public:
    Vector3(T _x = 0, T _y = 0, T _z = 0) : x(_x), y(_y), z(_z)
	{
	}

    Vector3(const Vector3<T> &v) : x(v.x), y(v.y), z(v.z)
	{
	}

    void set(const Vector3<T> &v)
	{
	    set(v.getX(), v.getY(), v.getZ());
	}

    void set(T _x, T _y, T _z)
	{
	    x = _x;
	    y = _y;
	    z = _z;
	}

    Vector3<T> operator*(T scalar) const
	{
	    return Vector3<T>(x * scalar, y * scalar, z * scalar);
	}

    Vector3<T> &operator*=(T scalar)
	{
	    return (*this = *this * scalar);
	}

    T operator*(const Vector3<T> &v) const
	{
	    return dot(v);
	}

    Vector3<T> &operator*=(const Vector3<T> &v)
	{
	    return (*this = dot(v));
	}

    Vector3<T> operator/(T scalar) const
	{
	    return *this * (1.0 / scalar);
	}

    Vector3<T> operator+(const Vector3<T> &v) const
	{
	    return Vector3<T>(x + v.x, y + v.y, z + v.z);
	}

    Point3<T> operator+(const Point3<T> &p) const
	{
	    return Point3<T>(p.getX() + x, p.getY() + y, p.getZ() + z);
	}

    Vector3<T> operator+=(const Vector3<T> &v)
	{
	    return (*this = *this + v);
	}

    Vector3<T> operator-(const Vector3<T> &v) const
	{
	    return Vector3<T>(x - v.x, y - v.y, z - v.z);
	}

    Vector3<T> operator-=(const Vector3<T> &v)
	{
	    return (*this = *this - v);
	}

    Vector3<T> operator-() const
	{
	    return Vector3<T>(-x, -y, -z);
	}

    T dot(const Vector3<T> &v) const
	{
	    return (x * v.getX() + y * v.getY() + z * v.getZ());
	}

    Vector3<T> cross(const Vector3<T> &v) const
	{
	    return Vector3<T>(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);
	}

    T length() const
	{
	    return sqrt(x * x + y * y + z * z);
	}
    
    Vector3<T> normal() const
	{
	    ASSERT(length() > 0);
	    return Vector3<T>(x / length(), y / length(), z / length());
	}
    
    void normalize()
	{
	    *this = normal();
	}
    
    Vector3<T> &operator=(const Vector3<T> &v)
	{
	    set(v);
	    return *this;
	}
    
    bool operator==(const Vector3<T> &v) const
	{
	    return (x == v.x && y == v.y && z == v.z);
	}
    
    bool operator!=(const Vector3<T> &v) const
	{
	    return !(*this == v);
	}
    
	bool operator!() const
	{
		return (x == 0 && y == 0 && z == 0);
	}

    T getX() const
	{
	    return x;
	}
    
    T getY() const
	{
	    return y;
	}

    T getZ() const
	{
	    return z;
	}

    void setX(T val)
	{
	    x = val;
	}

    void setY(T val)
	{
	    y = val;
	}

    void setZ(T val)
	{
	    z = val;
	}
    
    operator Point3<T>() const
	{
	    return Point3<T>(x, y, z);
	}
};

typedef Vector3<GLfloat> Vector3f;
typedef Vector3<GLint> Vector3i;
typedef Vector3<GLdouble> Vector3d;
typedef Vector3<GLshort> Vector3s;

inline void normal3f(const Vector3f &n)
{
    glNormal3f(n.getX(), n.getY(), n.getZ());
}

inline void normal3i(const Vector3i &n)
{
    glNormal3i(n.getX(), n.getY(), n.getZ());
}

inline void normal3d(const Vector3d &n)
{
    glNormal3d(n.getX(), n.getY(), n.getZ());
}

inline void normal3s(const Vector3s &n)
{
    glNormal3s(n.getX(), n.getY(), n.getZ());
}

inline void translatef(const Vector3f &t)
{
	glTranslatef(t.getX(), t.getY(), t.getZ());
}

inline void translated(const Vector3d &t)
{
	glTranslated(t.getX(), t.getY(), t.getZ());
}

#endif
