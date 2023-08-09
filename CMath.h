#pragma once
#include "Matrix.h"
#include "Matrix4x4.h"
#include "Vector3.h"
#include <cmath>

Vector3 Add(Vector3 a, Vector3 b);

Vector3 Multiply3(float scalar, const Vector3& v);
Vector3 Subtract(const Vector3& v1, const Vector3& v2);

float Dot(const Vector3& v1, const Vector3& v2);
float Length(const Vector3& v);
Vector3 Normalise(const Vector3& v);

Vector3 Slerp(const Vector3& s, const Vector3& e, float t);
Vector3 Lerp(const Vector3& s, const Vector3& e, float t);