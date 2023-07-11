#include "ParticleModel.h"
#include "Debugger.h"
#include "Quaternion.h"

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
	
	angularVelocity *= 0.5f;

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

XMMATRIX ParticleModel::GetInverseInertiaTensor()
{
	XMFLOAT4X4 _tensor;
	float rSquared = Radius * Radius;
	float fraction = 2.0f / 5.0f;
	_tensor._11 = rSquared * mass * fraction;
	_tensor._22 = rSquared * mass * fraction;
	_tensor._33 = rSquared * mass * fraction;
	_tensor._44 = 1.0f;

	_tensor._12 = 0.0f;
	_tensor._13 = 0.0f;
	_tensor._14 = 0.0f;
	_tensor._21 = 0.0f;
	_tensor._23 = 0.0f;
	_tensor._24 = 0.0f;
	_tensor._31 = 0.0f;
	_tensor._32 = 0.0f;
	_tensor._34 = 0.0f;
	_tensor._41 = 0.0f;
	_tensor._42 = 0.0f;
	_tensor._43 = 0.0f;

	XMMATRIX Tensor = XMLoadFloat4x4(&_tensor);

	XMMATRIX InverseTensor = XMMatrixInverse(nullptr, Tensor);

	return InverseTensor;
}

void ParticleModel::AddRotationalImpulse(Vector impulse, Vector normal)
{
	Vector point = normal * Radius;
	Vector torque = point.GetCrossProduct(impulse);

	XMFLOAT4X4 tensor; 
	XMMATRIX _tensor = GetInverseInertiaTensor();
	XMStoreFloat4x4(&tensor, _tensor);

	Vector acceleration;
	acceleration.X = torque.X * tensor._11;
	acceleration.Y = torque.Y * tensor._22;
	acceleration.Z = torque.Z * tensor._33;

	angularVelocity += acceleration;
}

XMMATRIX ParticleModel::GetAngularOrientation(float deltaTime)
{
	quat.addScaledVector(angularVelocity, deltaTime);
	quat.normalise();

	XMMATRIX matrix;
	CalculateTransformMatrixRowMajor(matrix, Vector(0.0f, 0.0f, 0.0f), quat);

	return matrix;
}