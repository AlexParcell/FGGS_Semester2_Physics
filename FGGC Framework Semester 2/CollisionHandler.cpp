#include "CollisionHandler.h"
#include "GameObject.h"
#include "Debugger.h"

CollisionHandler* CollisionHandler::Instance = nullptr;

Contact CollisionHandler::CheckCollision(ParticleModel* a, ParticleModel* b, float t)
{
	if (a == b || b->GetObjectType() == STATIC)
	{
		Contact contact;
		contact.Collided = false;
		return contact;
	}

	Transform* aTransform = a->GetGameObject()->GetTransform();
	Transform* bTransform = b->GetGameObject()->GetTransform();

	Contact contact;
	contact.A = a;
	contact.B = b;
	contact.deltaTime = t;

	float r = a->Radius + b->Radius;
	Vector d = bTransform->GetPosition() - aTransform->GetPosition();

	if (d.GetSquaredMagnitude() - r * r > 0
		|| d.GetSquaredMagnitude() == 0.0f)
	{
		return contact;
	}

	Vector n = d.GetNormalizedVector();

	contact.Collided = true;
	contact.hitNormal = d;
	contact.depth = fabsf(d.GetMagnitude() - r) * 0.5f;

	return contact;
}

void CollisionHandler::ResolveCollision(Contact collision)
{
	ParticleModel* a = collision.A;
	ParticleModel* b = collision.B;
	Transform* aTransform = a->GetGameObject()->GetTransform();
	Transform* bTransform = b->GetGameObject()->GetTransform();

	if (b->GetObjectType() == DYNAMIC)
	{
		Vector apos = aTransform->GetPosition();
		aTransform->SetPosition(apos - (collision.hitNormal * collision.depth));
		Vector av = (a->GetVelocity() * a->GetMass() + b->GetVelocity() * b->GetMass() + (b->GetVelocity() - a->GetVelocity()) * b->GetMass() * 0.5) / (a->GetMass() + b->GetMass());

		Vector bpos = bTransform->GetPosition();
		bTransform->SetPosition(bpos + (collision.hitNormal * collision.depth));
		Vector bv = (a->GetVelocity() * a->GetMass() + b->GetVelocity() * b->GetMass() + (b->GetVelocity() - a->GetVelocity()) * a->GetMass() * 0.5) / (a->GetMass() + b->GetMass());

		a->SetVelocity(av);
		b->SetVelocity(bv);
	}
}
/*
if (b->GetObjectType() == STATIC)
{
	a->AddForce(Vector(0, 10.0f, 0));
	Vector apos = aTransform->GetPosition();
	aTransform->SetPosition(apos - (collision.hitNormal * collision.depth));

	Vector av = a->GetVelocity();
	av.Y = 0;
	a->SetVelocity(av);
	Vector aa = a->GetAcceleration();
	aa.Y = 0;
	a->SetAcceleration(aa);
	return;
}
else
*/

void CollisionHandler::ResolveFloor(ParticleModel* a, float FloorHeight)
{
	Transform* aTransform = a->GetGameObject()->GetTransform();
	Vector aPos = aTransform->GetPosition();



	if (aPos.Y - a->Radius < FloorHeight)
	{
		a->AddForce(Vector(0, 10.0f, 0));

		// difference between position to radius takeaway position to floor height
		float DistanceToFloor = abs(aPos.Y - FloorHeight);
		float Depth = (aPos.Y + a->Radius) - DistanceToFloor;

		aTransform->SetPosition(Vector(aPos.X, aPos.Y - (aPos.Y - Depth), aPos.Z));

		Vector av = a->GetVelocity();
		av.Y = 0;
		a->SetVelocity(av);
		Vector aa = a->GetAcceleration();
		aa.Y = 0;
		a->SetAcceleration(aa);
	}
}