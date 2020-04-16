#include "CollisionHandler.h"
#include "GameObject.h"
#include "Debugger.h"

CollisionHandler* CollisionHandler::Instance = nullptr;

Contact CollisionHandler::CheckCollision(ParticleModel* a, ParticleModel* b, float t)
{
	if (a == b)
	{
		Contact contact;
		contact.Collided = false;
		return contact;
	}

	a->UpdateBoundingBox();
	b->UpdateBoundingBox();
	Vector amin = a->GetBoundingBox().LowerBound;
	Vector amax = a->GetBoundingBox().UpperBound;
	Vector bmin = b->GetBoundingBox().LowerBound;
	Vector bmax = b->GetBoundingBox().UpperBound;

	BoundingBox bbox = b->GetBoundingBox();

	Contact contact;
	contact.A = a;
	contact.B = b;
	contact.deltaTime = t;

	static const Vector faces[6]
	{
		Vector(-1, 0, 0),
		Vector(1, 0, 0),
		Vector(0, -1, 0),
		Vector(0, 1, 0),
		Vector(0, 0, -1),
		Vector(0, 0, 1)
	};

	float distances[6]
	{
		(bmax.X - amin.X),
		(amax.X - bmin.X),
		(bmax.Y - amin.Y),
		(amax.Y - bmin.Y),
		(bmax.Z - amin.Z),
		(amax.Z - bmin.Z)
	};

	float depth = 0.0f;
	Vector Normal;

	contact.Collided = true;

	for (int i = 0; i < 6; i++)
	{
		if (distances[i] < 0.0f)
		{
			contact.Collided = false;
			break;
		}

		if ((i == 0) || (distances[i] < depth))
		{
			depth = distances[i];
			Normal = faces[i];
		}
	}

	contact.hitNormal = Normal;
	contact.depth = depth;

	return contact;
}

void CollisionHandler::ResolveCollision(Contact collision)
{
	ParticleModel* a = collision.A;
	ParticleModel* b = collision.B;
	Transform* aTransform = a->GetGameObject()->GetTransform();
	Transform* bTransform = b->GetGameObject()->GetTransform();

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
	{
		Vector apos = aTransform->GetPosition();
		aTransform->SetPosition(apos - (collision.hitNormal * collision.depth));
		Vector av = (a->GetVelocity() * a->GetMass() + b->GetVelocity() * b->GetMass() + (b->GetVelocity() - a->GetVelocity()) * b->GetMass() * 0.5) / (a->GetMass() + b->GetMass());
		Debugger::GetInstance()->PrintVector("COLLISION NORMAL: ", collision.hitNormal);

		Vector bpos = bTransform->GetPosition();
		bTransform->SetPosition(bpos + (collision.hitNormal * collision.depth));
		Vector bv = (a->GetVelocity() * a->GetMass() + b->GetVelocity() * b->GetMass() + (b->GetVelocity() - a->GetVelocity()) * a->GetMass() * 0.5) / (a->GetMass() + b->GetMass());

		a->SetVelocity(av);
		b->SetVelocity(bv);
	}
}