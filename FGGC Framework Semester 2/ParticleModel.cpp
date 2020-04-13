#include "ParticleModel.h"


ParticleModel::ParticleModel(GameObject* Object) : _object(Object)
{

}
ParticleModel::~ParticleModel()
{

}

void ParticleModel::Update(float t)
{
	switch (mode)
	{
	case (MovementMode::ConstantVelocity):
		MoveConstantVelocity(t);
		break;
	case (MovementMode::ConstantAcceleration):
		MoveConstantAcceleration(t);
		break;
	case (MovementMode::DynamicAcceleration):
		{
		UpdateNetForce();
		UpdateAcceleration();
		Move(t);
		}
		break;
	}
}

void ParticleModel::MoveConstantVelocity(float t)
{
	Vector newPosition = _object->GetTransform()->GetPosition() + (_velocity * t);
	_object->GetTransform()->SetPosition(newPosition);
}
void ParticleModel::MoveConstantAcceleration(float t)
{
	Vector newPosition = _object->GetTransform()->GetPosition() + (_velocity * t) + (_acceleration * 0.5 * t * t);
	_object->GetTransform()->SetPosition(newPosition);

	_velocity = _velocity + _acceleration * t;
}

void ParticleModel::UpdateNetForce()
{
	for (int i = 0; i < forces.size(); i++)
	{
		_netForce += forces[i];
	}
	forces.clear();
}
void ParticleModel::UpdateAcceleration()
{
	_acceleration = _netForce / mass;
}
void ParticleModel::Move(float t)
{
	_velocity = (_velocity * pow(damping, t)) + _acceleration * t;
	debugger.PrintVector("Velocity: ", _velocity);

	Vector newPosition = _object->GetTransform()->GetPosition() + (_velocity * t) + (_acceleration * 0.5 * t * t);
	_object->GetTransform()->SetPosition(newPosition);

	_netForce = Vector(0.0f, 0.0f, 0.0f);
}