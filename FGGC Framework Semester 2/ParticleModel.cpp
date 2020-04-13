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
		AddForce(CalculateDrag());
		AddForce(Vector(0, -10, 0));
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
		debugger.PrintVector("Force: ", _netForce);
	}
	forces.clear();
}
void ParticleModel::UpdateAcceleration()
{
	_acceleration = _netForce / mass;
}
void ParticleModel::Move(float t)
{
	_velocity += _acceleration * t;

	Vector newPosition = _object->GetTransform()->GetPosition() + (_velocity * t) + (_acceleration * 0.5 * t * t);
	_object->GetTransform()->SetPosition(newPosition);

	_netForce = Vector(0.0f, 0.0f, 0.0f);
}

Vector ParticleModel::CalculateDrag()
{
	if (LaminarDragEnabled)
	{
		return LaminarDrag();
	}
	else
	{
		return TurbulentDrag();
	}
}
Vector ParticleModel::LaminarDrag()
{
	return _velocity * -dragCoefficient;
}
Vector ParticleModel::TurbulentDrag()
{
	float mag = _velocity.GetMagnitude();
	Vector unit = _velocity.GetNormalizedVector();

	float dragMag = dragCoefficient * mag * mag;

	return unit * -dragMag;
}

// AABB
bool ParticleModel::CollisionCheck(GameObject* otherObject)
{
	/*
	function intersect(a, b) {
		return (a.minX <= b.maxX && a.maxX >= b.minX) &&
			(a.minY <= b.maxY && a.maxY >= b.minY) &&
			(a.minZ <= b.maxZ && a.maxZ >= b.minZ);
	}
	*/

	Vector Position = _object->GetTransform()->GetPosition();
	Vector Scale = _object->GetBoundingBox();

	Vector OtherPosition = otherObject->GetTransform()->GetPosition();
	Vector OtherScale = otherObject->GetBoundingBox();

	return ((Position.X - Scale.X <= OtherPosition.X + OtherScale.X) && (Position.X + Scale.X >= OtherPosition.X - OtherScale.X)) &&
		((Position.Y - Scale.Y <= OtherPosition.Y + OtherScale.Y) && (Position.Y + Scale.Y >= OtherPosition.Y - OtherScale.Y)) &&
		((Position.Z - Scale.Z <= OtherPosition.Z + OtherScale.Z) && (Position.Z + Scale.Z >= OtherPosition.Z - OtherScale.Z));
}