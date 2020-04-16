#include "CollisionHandler.h"
#include "GameObject.h"
#include "Debugger.h"

CollisionHandler* CollisionHandler::Instance = nullptr;

Contact CollisionHandler::CheckCollision(ParticleModel* a, ParticleModel* b, float t)
{
	a->UpdateBoundingBox();
	b->UpdateBoundingBox();
	BoundingBox abox = a->GetBoundingBox();
	BoundingBox bbox = b->GetBoundingBox();

	Contact contact;
	contact.A = a;
	contact.B = b;
	contact.deltaTime = t;

	// AABB
	contact.Collided = (
			(abox.LowerBound.X <= bbox.UpperBound.X && abox.UpperBound.X >= bbox.LowerBound.X) &&
			(abox.LowerBound.Y <= bbox.UpperBound.Y && abox.UpperBound.Y >= bbox.LowerBound.Y) &&
			(abox.LowerBound.Z <= bbox.UpperBound.Z && abox.UpperBound.Z >= bbox.LowerBound.Z)
		);


	return contact;

}
void CollisionHandler::ResolveCollision(Contact collision)
{
	ParticleModel* a = collision.A;
	ParticleModel* b = collision.B;
	Vector aPos = a->GetGameObject()->GetTransform()->GetPosition();
	Vector bPos = b->GetGameObject()->GetTransform()->GetPosition();

	if (b->GetObjectType() == STATIC)
	{
		Vector av = a->GetVelocity();
		av.Y = (10 / a->GetMass()) * collision.deltaTime;
		a->SetVelocity(av);
	}
	else
	{
		//a->GetGameObject()->GetTransform()->RevertPosition();
		Vector av = (a->GetVelocity() * a->GetMass() + b->GetVelocity() * b->GetMass() + (b->GetVelocity() - a->GetVelocity()) * b->GetMass() * 0.5) / (a->GetMass() + b->GetMass());

		a->SetVelocity(av);


		//b->GetGameObject()->GetTransform()->RevertPosition();
		Vector bv = (a->GetVelocity() * a->GetMass() + b->GetVelocity() * b->GetMass() + (b->GetVelocity() - a->GetVelocity()) * a->GetMass() * 0.5) / (a->GetMass() + b->GetMass());

		b->SetVelocity(bv);
	}
}