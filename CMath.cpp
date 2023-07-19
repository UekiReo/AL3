#include "CMath.h"

Vector3 Add(Vector3 a, Vector3 b)
{
	return {a.x + b.x, a.y + b.y, a.z + b.z};
}

Vector3 Multiply3(float scalar, const Vector3& v) 
{
	return {v.x * scalar, v.y * scalar, v.z * scalar};
}

Vector3 Subtract(const Vector3& v1, const Vector3& v2) 
{
	return {v1.x - v2.x, v1.y - v2.y, v1.z - v2.z};
}

float Dot(const Vector3& v1, const Vector3& v2) 
{
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

float Length(const Vector3& v)
{
	return sqrtf((v.x * v.x) + (v.y * v.y) + (v.z * v.z)); 
}

Vector3 Normalise(const Vector3& v) 
{
	float len = Length(v);
	if (len != 0) 
	{
		return {v.x / len, v.y / len, v.z / len};
	}
	return v;
}

Vector3 Lerp(const Vector3& s, const Vector3& e, float t)
{
	Vector3 result;
	Vector3 es = Subtract(e, s);
	result = Add(s, Multiply3(t, es));
	return result;
}

Vector3 Slerp(const Vector3& s, const Vector3& e, float t)
{
	float dot = Dot(Normalise(s), Normalise(e));
	if (std::abs(dot) > 0.999f)
	{
		return Lerp(s, e, t);
	}
	float theta = std::acos(dot);
	float sinTheta = std::sin(theta);
	float t1 = std::sin((1.0f - t) * theta) / sinTheta;
	float t2 = std::sin(t * theta) / sinTheta;

	return Add(Multiply3(t1, s), Multiply3(t2, e));
}