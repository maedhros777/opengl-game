#include "Quaternion.h"

Quaternion Quaternion::operator*(const Quaternion &quat) const
{
	return Quaternion(w * quat.x + x * quat.w + y * quat.z - z * quat.y, w * quat.y + y * quat.w + z * quat.x - x * quat.z,
					  w * quat.z + z * quat.w + x * quat.y - y * quat.y, w * quat.w - x * quat.x - y * quat.y - z * quat.z);
}

Vector3f Quaternion::operator*(const Vector3f &vec) const
{
	Vector3f v = vec.normal();
	Quaternion quat = *this * (Quaternion(v.getX(), v.getY(), v.getZ(), 0.0) * conjugate());
	return Vector3f(quat.x, quat.y, quat.z);
}

Quaternion Quaternion::normal() const
{
	float len = length();
	ASSERT(len > 0.0);
	return Quaternion(x / len, y / len, z / len, w / len);
}

void Quaternion::normalize()
{
	float len = length();
	ASSERT(len > 0.0);
	x /= len;
	y /= len;
	z /= len;
	w /= len;
}

Quaternion slerp(const Quaternion &q1, const Quaternion &q2, float t)
{
	float cos_half_theta = q1.getW() * q2.getW() + q1.getX() * q2.getX() + q1.getY() * q2.getY() + q1.getZ() * q2.getZ();

	if (cos_half_theta >= 1.0 || cos_half_theta <= -1.0)
		return q1;

	float half_theta = acos(cos_half_theta);
	float sin_half_theta = sqrt(1.0 - cos_half_theta * cos_half_theta);

	if (sin_half_theta < 0.0001 && sin_half_theta > -0.0001)
		return Quaternion((q1.getW() + q2.getW()) / 2.0, (q1.getX() + q2.getX()) / 2.0, (q1.getY() + q2.getY()) / 2.0,
						  (q1.getZ() + q2.getZ()) / 2.0);

	float r1 = sin((1.0 - t) * half_theta) / sin_half_theta;
	float r2 = sin(t * half_theta) / sin_half_theta;
	return Quaternion(q1.getW() * r1 + q2.getW() * r2, q1.getX() * r1 + q2.getX() * r2, q1.getY() * r1 + q2.getY() * r2,
					  q1.getZ() * r1 + q2.getZ() * r2);
}

void mult_quat(const Quaternion &q)
{
	float x = q.getX(), y = q.getY(), z = q.getZ(), w = q.getW();
	float x2 = x * x, y2 = y * y, z2 = z * z;
	float xy = x * y, xz = x * z, yz = y * z, wx = w * x, wy = w * y, wz = w * z;
	float matrix[16] = { 1.0 - 2.0 * (y2 + z2), 2.0 * (xy + wz), 2.0 * (xz - wy), 0.0,
						 2.0 * (xy - wz), 1.0 - 2.0 * (x2 + z2), 2.0 * (yz + wx), 0.0,
						 2.0 * (xz + wy), 2.0 * (yz - wx), 1.0 - 2.0 * (x2 + y2), 0.0,
						 0.0, 0.0, 0.0, 1.0 };
	glMultMatrixf(matrix);
}

Quaternion to_quat(float mat[16])
{
	float trace = mat[0] + mat[5] + mat[10];
	float r, s;

	if (trace > TRACE_OFFSET)
	{
		r = sqrt(trace + 1.0);
		s = 0.5 / r;
		return Quaternion(0.5 * r, (mat[6] - mat[9]) * s, (mat[8] - mat[2]) * s, (mat[1] - mat[4]) * s);
	}

	else
	{
		float max_diagonal = std::max(mat[0], std::max(mat[5], mat[10]));

		if (max_diagonal == mat[0])
		{
			r = sqrt(1.0 + mat[0] - mat[5] - mat[10]);
			s = 0.5 / r;
			return Quaternion((mat[6] - mat[9]) * s, 0.5 * r, (mat[4] + mat[1]) * s, (mat[2] + mat[8]) * s);
		}

		else if (max_diagonal == mat[5])
		{
			r = sqrt(1.0 + mat[5] - mat[0] - mat[10]);
			s = 0.5 / r;
			return Quaternion((mat[8] - mat[2]) * s, (mat[4] + mat[1]) * s, 0.5 * r, (mat[9] + mat[6]) * s);
		}

		else
		{
			r = sqrt(1.0 + mat[10] - mat[0] - mat[5]);
			s = 0.5 / r;
			return Quaternion((mat[1] - mat[4]) * s, (mat[2] + mat[8]) * s, (mat[9] + mat[6]) * s, 0.5 * r);
		}
	}
}
