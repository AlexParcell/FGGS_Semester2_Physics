#include "ParticleModel.h"


ParticleModel::ParticleModel(GameObject* Object) : _object(Object)
{

}
ParticleModel::~ParticleModel()
{

}

void ParticleModel::Update(float t)
{

}

void ParticleModel::MoveConstantVelocity(float t)
{
	Vector newPosition = _object->GetTransform()->GetPosition() + (_velocity * t);
	_object->GetTransform()->SetPosition(newPosition);
}
void ParticleModel::MoveConstantAcceleration(float t)
{

}