#include "ParticleModel.h"
#include "Debugger.h"

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
		if (type == DYNAMIC)
		{
			AddForce(CalculateDrag());
			AddForce(Vector(0, -10 * mass, 0));
			UpdateNetForce();
			UpdateAcceleration();
			Move(t);
		}
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
	_velocity += _acceleration * t;
	_velocity = _velocity.Truncate(MaxSpeed);

	Vector newPosition = _object->GetTransform()->GetPosition() + (_velocity * t);
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

void ParticleModel::UpdateBoundingBox()
{
	Vector boxDimensions = _object->GetBoundingBox();
	Vector position = _object->GetTransform()->GetPosition();

	bb.LowerBound =	Vector(
			position.X - boxDimensions.X,
			position.Y - boxDimensions.Y,
			position.Z - boxDimensions.Z
			);

	bb.UpperBound = Vector(
		position.X + boxDimensions.X,
		position.Y + boxDimensions.Y,
		position.Z + boxDimensions.Z
	);
}