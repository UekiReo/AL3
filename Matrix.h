#pragma once
#include "Matrix4x4.h"
#include "Vector3.h"
#include <assert.h>
#include <cmath>

// Vector3同士の足し算
Vector3 VectorAdd(const Vector3& translation, const Vector3& move);

// 1 X軸回転行列
Matrix4x4 MakeRotateXmatrix(float radian);

// 2 Y軸回転行列
Matrix4x4 MakeRotateYmatrix(float radian);

// 3 Z軸回転行列
Matrix4x4 MakeRotateZmatrix(float radian);

// 平行移動
Matrix4x4 MakeTranslateMatrix(Vector3 translate);

// 拡大縮小
Matrix4x4 MakeScaleMatrix(const Vector3& scale);

Matrix4x4 Multiply(const Matrix4x4& m1, const Matrix4x4& m2);

// TransformNormal
Vector3 TransformNormal(const Vector3& v, const Matrix4x4& m);

// アフィン変換
Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate);