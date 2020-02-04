#pragma once

#include "Vector.h"
#include "GameObject.h"

class ParticleModel
{
	Vector _acceleration = Vector(0.0f, 0.0f, 0.0f);
	Vector _velocity = Vector(0.0f, 0.0f, 0.0f);
	GameObject* _object = nullptr;

public:
	ParticleModel(GameObject* Object);
	~ParticleModel();

	void Update(float t);

	void MoveConstantVelocity(float t);
	void MoveConstantAcceleration(float t);
};

