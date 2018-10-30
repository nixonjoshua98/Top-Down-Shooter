
#include "stdafx.h"
#include "vector2.h"
#include <math.h>

Vector2::Vector2(float x, float y)
{
	this->x = x;
	this->y = y;
}

Vector2::Vector2()
{
	x = 0;
	y = 0;
}

float Vector2::Magnitude()
{
	return sqrtf(pow(x, 2) + pow(y, 2));
}

Vector2 Vector2::Normalise()
{
	float m = Magnitude();

	return Vector2(x / m, y / m);
}