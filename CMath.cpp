#include "CMath.h"

Vector3 Add(Vector3 a, Vector3 b) { return {a.x + b.x, a.y + b.y, a.z + b.z}; }

Vector3 Multiply3(float scalar, const Vector3& v) {
	return {v.x * scalar, v.y * scalar, v.z * scalar};
}

Vector3 Subtract(const Vector3& v1, const Vector3& v2) {
	return {v1.x - v2.x, v1.y - v2.y, v1.z - v2.z};
}

float Dot(const Vector3& v1, const Vector3& v2) { return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z; }
float Length(const Vector3& v) { return sqrtf((v.x * v.x) + (v.y * v.y) + (v.z * v.z)); }

Matrix4x4
    MakeViewportMatrix(float left, float top, float width, float height, float min, float max) {
	Matrix4x4 result;
	result.m[0][0] = width / 2;
	result.m[0][1] = 0.0f;
	result.m[0][2] = 0.0f;
	result.m[0][3] = 0.0f;

	result.m[1][0] = 0.0f;
	result.m[1][1] = -(height / 2);
	result.m[1][2] = 0.0f;
	result.m[1][3] = 0.0f;

	result.m[2][0] = 0.0f;
	result.m[2][1] = 0.0f;
	result.m[2][2] = max - min;
	result.m[2][3] = 0.0f;

	result.m[3][0] = left + (width / 2);
	result.m[3][1] = top + (height / 2);
	result.m[3][2] = min;
	result.m[3][3] = 1.0f;
	return result;
}

Vector3 Normalise(const Vector3& v) {
	float len = Length(v);
	if (len != 0) {
		return {v.x / len, v.y / len, v.z / len};
	}
	return v;
}

Vector3 Lerp(const Vector3& s, const Vector3& e, float t) {
	Vector3 result;
	Vector3 es = Subtract(e, s);
	result = Add(s, Multiply3(t, es));
	return result;
}
Vector3 Slerp(const Vector3& s, const Vector3& e, float t) {
	float dot = Dot(Normalise(s), Normalise(e));
	if (std::abs(dot) > 0.999f) {
		return Lerp(s, e, t);
	}
	float theta = std::acos(dot);
	float sinTheta = std::sin(theta);
	float t1 = std::sin((1.0f - t) * theta) / sinTheta;
	float t2 = std::sin(t * theta) / sinTheta;

	return Add(Multiply3(t1, s), Multiply3(t2, e));
}

Vector3 Multiply(float scalar, const Vector3& v) {
	return {v.x * scalar, v.y * scalar, v.z * scalar};
}

Vector3 Transform(const Vector3& vector, const Matrix4x4& matrix) {
	Vector3 result;
	result.x = vector.x * matrix.m[0][0] + vector.y * matrix.m[1][0] + vector.z * matrix.m[2][0] +
	           1.0f * matrix.m[3][0];
	result.y = vector.x * matrix.m[0][1] + vector.y * matrix.m[1][1] + vector.z * matrix.m[2][1] +
	           1.0f * matrix.m[3][1];
	result.z = vector.x * matrix.m[0][2] + vector.y * matrix.m[1][2] + vector.z * matrix.m[2][2] +
	           1.0f * matrix.m[3][2];
	float w = vector.x * matrix.m[0][3] + vector.y * matrix.m[1][3] + vector.z * matrix.m[2][3] +
	          1.0f * matrix.m[3][3];
	assert(w != 0.0f);
	result.x /= w;
	result.y /= w;
	result.z /= w;
	return result;
}