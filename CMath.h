#pragma once
#include "Matrix4x4.h"
#include "Vector3.h"
#include <cmath>
#include "Matrix.h"

Vector3 Add(Vector3 a, Vector3 b);

Vector3 Multiply3(float scalar, const Vector3& v);
Vector3 Subtract(const Vector3& v1, const Vector3& v2);

float Dot(const Vector3& v1, const Vector3& v2);
float Length(const Vector3& v);

Matrix4x4 MakeViewportMatrix(float left, float top, float width, float height, float min, float max);

Vector3 Normalise(const Vector3& v);

Vector3 Slerp(const Vector3& s, const Vector3& e, float t);
Vector3 Lerp(const Vector3& s, const Vector3& e, float t);

Vector3 Multiply(float scalar, const Vector3& v);

Vector3 Transform(const Vector3& vector, const Matrix4x4& matrix);