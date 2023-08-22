#pragma once
#include "Matrix4x4.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include <assert.h>
#include <cmath>

Vector3 Add(const Vector3& v1, const Vector3& v2);

Vector3 Subtract(const Vector3& v1, const Vector3& v2);

Vector3 Multiply(const float& v1, const Vector3& v2);

Vector3 VectorScale(const Vector3& v1, const float& v2);

float Dot(const Vector3& v1, const Vector3& v2);

float Length(const Vector3& v);

// Vector3同士の足し算
Vector3 VectorAdd(const Vector3& translation, const Vector3& move);

// 回転行列
Matrix4x4 MakeRotateMatrix(const Vector3& radian);

// X軸回転行列
Matrix4x4 MakeRotateXmatrix(float radian);

// Y軸回転行列
Matrix4x4 MakeRotateYmatrix(float radian);

// Z軸回転行列
Matrix4x4 MakeRotateZmatrix(float radian);

// 平行移動
Matrix4x4 MakeTranslateMatrix(Vector3 translate);

// 拡大縮小
Matrix4x4 MakeScaleMatrix(const Vector3& scale);

// 正規化
Matrix4x4 Multiply(const Matrix4x4& m1, const Matrix4x4& m2);

// TransformNormal
Vector3 TransformNormal(const Vector3& v, const Matrix4x4& m);

// アフィン変換
Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate);

Vector3 Normalize(const Vector3& v);

Matrix4x4 Inverse(const Matrix4x4& m);