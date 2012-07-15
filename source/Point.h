#ifndef POINT_H
#define POINT_H
#include <iostream>
#include "Vector.h"

template <class T>
class Point2
{
private:
    T x, y;
public:
    Point2(T _x = 0, T _y = 0) : x(_x), y(_y)
    {
    }
    Point2(const Point2<T> &p) : x(p.x), y(p.y)
    {
    }
    void set(T _x, T _y)
    {
        x = _x;
        y = _y;
    }
    void set(const Point2<T> &p)
    {
        set(p.x, p.y);
    }
    Vector2<T> operator-(const Point2<T> &p) const
    {
        return Vector2<T>(x - p.x, y - p.y);
    }
    Point2<T> operator-(const Vector2<T> &v) const
    {
        return Point2<T>(x - v.getX(), y - v.getY());
    }
    Point2<T> &operator-=(const Vector2<T> &v)
    {
        return (*this = *this - v);
    }
    Point2<T> operator-() const
    {
        return Point2<T>(-x, -y);
    }
    Point2<T> operator+(const Vector2<T> &v) const
    {
        return Point2<T>(x + v.getX(), y + v.getY());
    }
    Point2<T> &operator+=(const Vector2<T> &v)
    {
        return (*this = *this + v);
    }
	Point2<T> operator*(const T scalar) const
	{
		return Point2<T>(x * scalar, y * scalar);
	}
	Point2<T> &operator*=(const T scalar)
	{
		return (*this = *this * scalar);
	}
    Point2<T> &operator=(const Point2<T> &p)
    {
        set(p);
        return *this;
    }
    bool operator==(const Point2<T> &p) const
    {
        return (x == p.x && y == p.y);
    }
    bool operator!=(const Point2<T> &p) const
    {
        return !(*this == p);
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
    void setX(T x)
    {
        this->x = x;
    }
    void setY(T y)
    {
        this->y = y;
    }
};

typedef Point2<GLfloat> Point2f;
typedef Point2<GLint> Point2i;
typedef Point2<GLdouble> Point2d;
typedef Point2<GLshort> Point2s;

inline void texCoord2f(const Point2f &p)
{
    glTexCoord2f(p.getX(), p.getY());
}

inline void texCoord2i(const Point2i &p)
{
    glTexCoord2i(p.getX(), p.getY());
}

inline void texCoord2d(const Point2d &p)
{
    glTexCoord2d(p.getX(), p.getY());
}

inline void texCoord2s(const Point2s &p)
{
    glTexCoord2s(p.getX(), p.getY());
}

template <class T>
class Point3
{
private:
    T x, y, z;
public:
    Point3(T _x = 0, T _y = 0, T _z = 0) : x(_x), y(_y), z(_z)
    {
    }
    Point3(const Point3<T> &p) : x(p.x), y(p.y), z(p.z)
    {
    }
    void set(T _x, T _y, T _z)
    {
        x = _x;
        y = _y;
        z = _z;
    }
    void set(const Point3<T> &p)
    {
        set(p.x, p.y, p.z);
    }
    Vector3<T> operator-(const Point3<T> &p) const
    {
        return Vector3<T>(x - p.x, y - p.y, z - p.z);
    }
    Point3<T> operator-() const
    {
        return Point3<T>(-x, -y, -z);
    }
    Point3<T> operator+(const Vector3<T> &v) const
    {
        return Point3<T>(x + v.getX(), y + v.getY(), z + v.getZ());
    }
    Point3<T> &operator+=(const Vector3<T> &v)
    {
        return (*this = *this + v);
    }
	Point3<T> operator*(T scalar) const
	{
		return Point3<T>(x * scalar, y * scalar, z * scalar);
	}
	Point3<T> &operator*=(T scalar)
	{
		return (*this = *this * scalar);
	}
    Point3<T> &operator=(const Point3<T> &p)
    {
        set(p);
        return *this;
    }
    bool operator==(const Point3<T> &p) const
    {
        return (x == p.x && y == p.y && z == p.z);
    }
    bool operator!=(const Point3<T> &p) const
    {
        return !(*this == p);
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
    void setX(T x)
    {
        this->x = x;
    }
    void setY(T y)
    {
        this->y = y;
    }
    void setZ(T z)
    {
        this->z = z;
    }

    operator Vector3<T>() const
    {
        return Vector3<T>(x, y, z);
    }
};

typedef Point3<GLfloat> Point3f;
typedef Point3<GLint> Point3i;
typedef Point3<GLdouble> Point3d;
typedef Point3<GLshort> Point3s;

inline void vertex3f(const Point3f &p)
{
    glVertex3f(p.getX(), p.getY(), p.getZ());
}

inline void vertex3i(const Point3i &p)
{
    glVertex3i(p.getX(), p.getY(), p.getZ());
}

inline void vertex3d(const Point3d &p)
{
    glVertex3d(p.getX(), p.getY(), p.getZ());
}

inline void vertex3s(const Point3s &p)
{
    glVertex3s(p.getX(), p.getY(), p.getZ());
}

template <class T>
void disp(const Point3<T> &p)
{
	std::cout << "point (" << p.getX() << ", " << p.getY() << ", " << p.getZ() << ")" << std::endl;
}

#endif
