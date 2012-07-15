#ifndef CAMERA_H
#define CAMERA_H
#include "Point.h"
#include "Vector.h"
#include "Math.h"

class Camera //TODO: add z-rotation
{
protected:
	Point3f pos;
	float x_rot, y_rot;
public:
	Camera(const Point3f &p = Point3f()) : pos(p), x_rot(0.0), y_rot(0.0)
	{
	}

	void orient(bool translate = true) const;
	void rotateX(float rot);
    void rotateY(float rot);
	virtual void advance(float amount);

    void orient_before() const
    {
        orient(false);
    }

    void set_pos(const Point3f &p)
    {
        pos = p;
    }

    Point3f get_pos() const
    {
        return pos;
    }
};

class FPCamera : public Camera
{
public:
	FPCamera(const Point3f &p = Point3f()) : Camera(p)
	{
	}

	void strafe(float amount);
	void advance(float amount);
};

#endif
