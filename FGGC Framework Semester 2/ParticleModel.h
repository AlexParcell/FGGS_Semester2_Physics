#pragma once

#include "Vector.h"
#include "GameObject.h"

class ParticleModel
{
	Vector _acceleration = Vector(0.0f, 0.0f, 0.0f);
	Vector _velocity = Vector(0.0f, 0.0f, 0.0f);
	GameObject* _object = nullptr;
	bool _accelerate = false;

public:
	ParticleModel(GameObject* Object);
	~ParticleModel();

	void Update(float t);

	void MoveConstantVelocity(float t);
	void MoveConstantAcceleration(float t);

	Vector GetAcceleration() { return _acceleration; }
	Vector GetVelocity() { return _velocity; }
	void SetAcceleration(Vector v) { _acceleration = v; }
	void SetVelocity(Vector v) { _velocity = v; }
};

