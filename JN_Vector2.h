#ifndef JN_VECTOR_2_H
#define JN_VECTOR_2_H

class JN_Vector2
{
public:
	JN_Vector2();
	JN_Vector2(float x, float y);

	float x;
	float y;

	float Magnitude();
	JN_Vector2 Normalise();

	JN_Vector2 operator / (int i);
};


#endif // !JN_VECTOR_2
