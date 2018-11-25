
#include "stdafx.h"

#include "JN_Vector2.h"

#include <math.h>

JN_Vector2::JN_Vector2(float x, float y)
{
	this->x = x;
	this->y = y;
}

// Set both x and y to zero by default
JN_Vector2::JN_Vector2()
{
	x = 0;
	y = 0;
}


// Returns the magnitude of the vector
float JN_Vector2::Magnitude()
{
	return sqrtf((float)pow(x, 2) + (float)pow(y, 2));
}


// Returns a new Vector2 object normalised
JN_Vector2 JN_Vector2::Normalise()
{
	float m = Magnitude();

	return JN_Vector2(x / m, y / m);
}