#pragma once

#include "Vector.h"
#include "GameObject.h"
#include <vector>
#include "Quaternion.h"

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
	float MaxSpeed = 10.0f;
	Vector angularVelocity;
	Quaternion quat = Quaternion();

public:
	ParticleModel(GameObject* Object);
	~ParticleModel();

	void Update(float t);

	float Radius = 0.5f;

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

	void AddRotationalImpulse(Vector impulse, Vector normal);

	void AddForce(Vector f) { forces.push_back(f); }

	Vector CalculateDrag();
	Vector LaminarDrag();
	Vector TurbulentDrag();

	XMMATRIX GetInverseInertiaTensor();

	XMMATRIX GetAngularOrientation(float deltaTime);

	float GetMass() { return mass; }

	Vector GetAcceleration() { return _acceleration; }
	Vector GetVelocity() { return _velocity; }
	void SetAcceleration(Vector v) { _acceleration = v; }
	void SetVelocity(Vector v) { _velocity = v; }
};

