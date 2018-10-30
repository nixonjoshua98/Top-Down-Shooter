#ifndef VECTOR_2
#define VECTOR_2

class Vector2
{
public:
	Vector2(float x, float y);
	Vector2();

	float x, y;

	float Magnitude();
	Vector2 Normalise();
};


#endif // !VECTOR_2
