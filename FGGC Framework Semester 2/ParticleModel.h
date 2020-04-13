#pragma once

#include "Vector.h"
#include "GameObject.h"
#include <vector>
#include "Debugger.h"

using namespace std;

enum MovementMode
{
	ConstantVelocity = 0,
	ConstantAcceleration,
	DynamicAcceleration
};

class ParticleModel
{
	Vector _acceleration = Vector(0.0f, 0.0f, 0.0f);
	Vector _velocity = Vector(0.0f, 0.0f, 0.0f);
	GameObject* _object = nullptr;
	MovementMode mode = MovementMode::DynamicAcceleration;
	bool LaminarDragEnabled = true;
	Vector _netForce;
	vector<Vector> forces;
	float mass = 1.0f;
	float dragCoefficient = 1.05;

	const float GroundThreshold = 0.0f;

	Debugger debugger;

public:
	ParticleModel(GameObject* Object);
	~ParticleModel();

	void Update(float t);

	void MoveConstantVelocity(float t);
	void MoveConstantAcceleration(float t);

	void UpdateNetForce();
	void UpdateAcceleration();
	void Move(float t);

	void AddForce(Vector f) { forces.push_back(f); }

	Vector CalculateDrag();
	Vector LaminarDrag();
	Vector TurbulentDrag();

	Vector GetAcceleration() { return _acceleration; }
	Vector GetVelocity() { return _velocity; }
	void SetAcceleration(Vector v) { _acceleration = v; }
	void SetVelocity(Vector v) { _velocity = v; }

	bool CollisionCheck(GameObject* otherObject);
};

