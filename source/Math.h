#ifndef MATH_H
#define MATH_H
#include <cmath>

const double PI = 3.14159265358979;

inline float dsin(float theta)
{
	return sin(theta * PI / 180.0);
}

inline float dcos(float theta)
{
	return cos(theta * PI / 180.0);
}

#endif
