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
		Vector av = (a->GetVelocity() * a->GetMass() + b->GetVelocity() * b->GetMass() + (b->GetVelocity() - a->GetVelocity()) * b->GetMass() * 2.0) / (a->GetMass() + b->GetMass());
		av = av * (collision.hitNormal * -1);

		Vector bpos = bTransform->GetPosition();
		bTransform->SetPosition(bpos + (collision.hitNormal * collision.depth));
		Vector bv = (a->GetVelocity() * a->GetMass() + b->GetVelocity() * b->GetMass() + (b->GetVelocity() - a->GetVelocity()) * a->GetMass() * 2.0) / (a->GetMass() + b->GetMass());
		bv = bv * collision.hitNormal;

		a->AddRotationalImpulse(av, collision.hitNormal);
		a->SetVelocity(av);
		b->AddRotationalImpulse(bv, collision.hitNormal);
		b->SetVelocity(bv);
	}
}

void CollisionHandler::ResolveFloor(ParticleModel* a, float FloorHeight)
{
	Transform* aTransform = a->GetGameObject()->GetTransform();
	Vector aPos = aTransform->GetPosition();

	if (aPos.Y - a->Radius < FloorHeight)
	{
		a->AddForce(Vector(0, 10.0f * a->GetMass() , 0));

		// difference between position to radius takeaway position to floor height
		float DistanceToFloor = abs(aPos.Y - FloorHeight);
		float Depth = (aPos.Y + a->Radius) - DistanceToFloor;

		//a->AddRotationalImpulse(Vector(0, 10.0f * a->GetMass(), 0), Vector(0, -1, 0));
		aTransform->SetPosition(Vector(aPos.X, aPos.Y - (aPos.Y - Depth), aPos.Z));

		Vector av = a->GetVelocity();
		av.Y = 0;
		a->SetVelocity(av);
		Vector aa = a->GetAcceleration();
		aa.Y = 0;
		a->SetAcceleration(aa);
		a->AddRotationalImpulse(av, Vector(0, 1, 0));
	}
}

void CollisionHandler::ResolveFloorAsWater(ParticleModel* a, float FloorHeight)
{
	Transform* aTransform = a->GetGameObject()->GetTransform();
	Vector aPos = aTransform->GetPosition();

	if (aPos.Y - a->Radius < FloorHeight)
	{
		// difference between position to radius takeaway position to floor height
		float DistanceToFloor = abs(aPos.Y - FloorHeight);
		float Depth = (aPos.Y + a->Radius) - DistanceToFloor;
		float SphericalCap = (3.14 * pow(Depth, 2.0f)) / 3.0f * (3.0f * (a->Radius - Depth));
		float FluidDensity = 5;

		Vector Buoyancy = Vector(0, 10 * a->GetMass(), 0) * SphericalCap * FluidDensity;

		a->AddForce(Buoyancy);
	}
}

void CollisionHandler::ResolveWalls(ParticleModel* a, float minX, float maxX, float minZ, float maxZ)
{
	Transform* aTransform = a->GetGameObject()->GetTransform();
	Vector aPos = aTransform->GetPosition();

	if (aPos.X - a->Radius < minX)
	{
		// difference between position to radius takeaway position to floor height
		float DistanceToWall = abs(aPos.X - minX);
		float Depth = (aPos.X + a->Radius) - DistanceToWall;

		aTransform->SetPosition(Vector(aPos.X - (aPos.X - Depth), aPos.Y, aPos.Z));

		Vector av = a->GetVelocity();
		av.X *= -1;
		a->SetVelocity(av);
		Vector aa = a->GetAcceleration();
		aa.X *= -1;
		a->SetAcceleration(aa);
	}

	if (aPos.X + a->Radius > maxX)
	{
		// difference between position to radius takeaway position to floor height
		float DistanceToWall = abs(aPos.X - maxX);
		float Depth = (aPos.X + a->Radius) - DistanceToWall;

		aTransform->SetPosition(Vector(aPos.X + (aPos.X - Depth), aPos.Y, aPos.Z));

		Vector av = a->GetVelocity();
		av.X *= -1;
		a->SetVelocity(av);
		Vector aa = a->GetAcceleration();
		aa.X *= -1;
		a->SetAcceleration(aa);
	}

	if (aPos.Z - a->Radius < minZ)
	{
		// difference between position to radius takeaway position to floor height
		float DistanceToWall = abs(aPos.Z - minZ);
		float Depth = (aPos.Z + a->Radius) - DistanceToWall;

		aTransform->SetPosition(Vector(aPos.X, aPos.Y, aPos.Z - (aPos.Z - Depth)));

		Vector av = a->GetVelocity();
		av.Z *= -1;
		a->SetVelocity(av);
		Vector aa = a->GetAcceleration();
		aa.Z *= -1;
		a->SetAcceleration(aa);
	}

	if (aPos.Z + a->Radius > maxZ)
	{
		// difference between position to radius takeaway position to floor height
		float DistanceToWall = abs(aPos.Z - maxZ);
		float Depth = (aPos.Z + a->Radius) - DistanceToWall;

		aTransform->SetPosition(Vector(aPos.X, aPos.Y, aPos.Z + (aPos.Z - Depth)));

		Vector av = a->GetVelocity();
		av.Z *= -1;
		a->SetVelocity(av);
		Vector aa = a->GetAcceleration();
		aa.Z *= -1;
		a->SetAcceleration(aa);
	}
}