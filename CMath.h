#pragma once
#include "Matrix4x4.h"
#include "Vector3.h"
#include <math.h>

Matrix4x4 MakeRotateXMatrix(float theta);
Matrix4x4 MakeRotateYMatrix(float theta);

Matrix4x4 MakeRotateZMatrix(float theta);
Vector3 Add(Vector3 a, Vector3 b);

Vector3 Subtract(const Vector3& v1, const Vector3& v2);
float Dot(const Vector3& v1, const Vector3& v2);
float Length(const Vector3& v);
Vector3 Normalise(const Vector3& v);