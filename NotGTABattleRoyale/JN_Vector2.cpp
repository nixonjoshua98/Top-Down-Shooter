
#include "stdafx.h"

#include "JN_Vector2.h"

#include <math.h>

JN_Vector2::JN_Vector2(float x, float y)
{
	this->x = x;
	this->y = y;
}

JN_Vector2::JN_Vector2()
{
	x = 0;
	y = 0;
}

float JN_Vector2::Magnitude()
{
	return sqrtf(pow(x, 2) + pow(y, 2));
}

JN_Vector2 JN_Vector2::Normalise()
{
	float m = Magnitude();

	return JN_Vector2(x / m, y / m);
}