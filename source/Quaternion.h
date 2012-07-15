#ifndef QUATERNION_H
#define QUATERNION_H

#include "Vector.h"
#include <cmath>
#include <algorithm>

const float TRACE_OFFSET = 0.0001;
const float EQUALITY_OFFSET = 0.001;

class Quaternion
{
private:
	float w, x, y, z;
public:
	Quaternion()
	{
	}

	Quaternion(float _w, float _x, float _y, float _z) : w(_w), x(_x), y(_y), z(_z)
	{
	}

	Quaternion conjugate() const
	{
		return Quaternion(-x, -y, -z, w);
	}

	float length() const
	{
		return sqrt(x * x + y * y + z * z + w * w);
	}

	float getX() const
	{
		return x;
	}

	float getY() const
	{
		return y;
	}

	float getZ() const
	{
		return z;
	}

	float getW() const
	{
		return w;
	}

	bool operator==(const Quaternion &quat) const
	{
		return (fabs(w - quat.w) < EQUALITY_OFFSET && fabs(x - quat.x) < EQUALITY_OFFSET && fabs(y - quat.y) < EQUALITY_OFFSET &&
				fabs(z - quat.z) < EQUALITY_OFFSET);
	}

	bool operator!=(const Quaternion &quat) const
	{
		return !(*this == quat);
	}

	Quaternion operator*(const Quaternion &quat) const;
	Vector3f operator*(const Vector3f &vec) const;
	Quaternion normal() const;
	void normalize();

	Quaternion inverse() const
	{
		return Quaternion(w, -x, -y, -z);
	}

	Quaternion &operator*=(const Quaternion &quat)
	{
		return (*this = *this * quat);
	}
};

Quaternion slerp(const Quaternion &q1, const Quaternion &q2, float t);
void mult_quat(const Quaternion &q);
Quaternion to_quat(float mat[16]);

#endif
