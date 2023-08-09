#pragma once
#include "Vector3.h"

class Collider 
{
private:
	float radius_ = 1.0f;

public:
	float Getradius() const { return radius_; }
	void SetRadius(float rad) { radius_ = rad; }
	virtual ~Collider() {}
	virtual void OnCollision() = 0;
	virtual Vector3 GetWorldPosition() = 0;
};