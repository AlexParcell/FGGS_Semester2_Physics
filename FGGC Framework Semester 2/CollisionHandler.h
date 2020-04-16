#pragma once

#include "ParticleModel.h"
#include "Vector.h"

struct Contact
{
	ParticleModel* A;
	ParticleModel* B;
	bool Collided = false;
	float deltaTime = 0.0f;
	Vector hitNormal;
	float depth;
};

class CollisionHandler
{
	static CollisionHandler* Instance;

public:
	static CollisionHandler* GetInstance()
	{
		if (Instance != nullptr)
		{
			Instance = new CollisionHandler();
		}

		return Instance;
	}

	Contact CheckCollision(ParticleModel* a, ParticleModel* b, float t);
	void ResolveCollision(Contact collision);
	void ResolveFloor(ParticleModel* a, float FloorHeight);
	void CollisionHandler::ResolveWalls(ParticleModel* a, float minX, float maxX, float minZ, float maxZ);
};

