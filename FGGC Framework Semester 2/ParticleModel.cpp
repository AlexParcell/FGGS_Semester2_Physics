#include "ParticleModel.h"


ParticleModel::ParticleModel(GameObject* Object) : _object(Object)
{

}
ParticleModel::~ParticleModel()
{

}

void ParticleModel::Update(float t)
{
	if (_accelerate)
	{
		MoveConstantAcceleration(t);
	}
	else
	{
		MoveConstantVelocity(t);
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