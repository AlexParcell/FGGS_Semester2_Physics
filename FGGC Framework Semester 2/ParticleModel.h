#pragma once

#include "Vector.h"
#include "GameObject.h"
#include <vector>

using namespace std;

enum MovementMode
{
	ConstantVelocity = 0,
	ConstantAcceleration,
	DynamicAcceleration
};

enum ObjectType
{
	DYNAMIC = 0,
	STATIC
};

struct BoundingBox
{
	Vector UpperBound;
	Vector LowerBound;
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
	ObjectType type = DYNAMIC;
	bool Grounded = false;

	BoundingBox bb;

public:
	ParticleModel(GameObject* Object);
	~ParticleModel();

	void Update(float t);

	void SetGrounded(bool g) { Grounded = g; }
	bool GetGrounded() { return Grounded; }

	void MoveConstantVelocity(float t);
	void MoveConstantAcceleration(float t);

	void UpdateNetForce();
	void UpdateAcceleration();
	void Move(float t);

	ObjectType GetObjectType() { return type; }
	void SetObjectType(ObjectType t) { type = t; }

	GameObject* GetGameObject() { return _object; }

	void AddForce(Vector f) { forces.push_back(f); }

	Vector CalculateDrag();
	Vector LaminarDrag();
	Vector TurbulentDrag();

	void UpdateBoundingBox();
	BoundingBox GetBoundingBox() { return bb; }

	float GetMass() { return mass; }

	Vector GetAcceleration() { return _acceleration; }
	Vector GetVelocity() { return _velocity; }
	void SetAcceleration(Vector v) { _acceleration = v; }
	void SetVelocity(Vector v) { _velocity = v; }

	//Vector ResolveCollisions();

	//bool CollisionCheck(GameObject* otherObject);
};

