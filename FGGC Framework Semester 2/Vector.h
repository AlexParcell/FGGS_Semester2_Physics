#pragma once
#include <cmath>
#include <math.h>
#include <directxmath.h>
#include <d3d11_1.h>

using namespace DirectX;

class Vector
{
public:
	float X, Y, Z;

	Vector()
	{
		X = 0.0f;
		Y = 0.0f;
		Z = 0.0f;
	}

	Vector(float x, float y, float z)
	{
		X = x;
		Y = y;
		Z = z;
	}

	Vector(const Vector &OtherVector)
	{
		X = OtherVector.X;
		Y = OtherVector.Y;
		Z = OtherVector.Z;
	}

	Vector(XMFLOAT3 OtherVector)
	{
		X = OtherVector.x;
		Y = OtherVector.y;
		Z = OtherVector.z;
	}

	~Vector()
	{

	}

	Vector GetCrossProduct(Vector OtherVector)
	{
		return Vector(
			(Y * OtherVector.Z) - (Z * OtherVector.Y),
			(Z * OtherVector.X) - (X * OtherVector.Z),
			(X * OtherVector.Y) - (Y * OtherVector.X)
		);
	}

	float GetDotProduct(Vector OtherVector)
	{
		return (X * OtherVector.X) + (Y * OtherVector.Y) + (Z * OtherVector.Z);
	}

	Vector GetNormalizedVector()
	{
		float mag = sqrt(pow(X, 2.0f) + pow(Y, 2.0f) + pow(Z, 2.0f));
		return Vector(
			X / mag,
			Y / mag,
			Z / mag
		);
	}

	float GetMagnitude()
	{
		return sqrt(pow(X, 2.0f) + pow(Y, 2.0f) + pow(Z, 2.0f));
	}

	Vector operator*(float N)
	{
		return Vector
		{
			X * N,
			Y * N,
			Z * N
		};
	}

	void operator*=(float N)
	{
		X *= N;
		Y *= N;
		Z *= N;
	}

	Vector operator+(Vector OtherVector)
	{
		return Vector(
			X + OtherVector.X,
			Y + OtherVector.Y,
			Z + OtherVector.Z
		);
	}

	void operator+=(Vector OtherVector)
	{
		X += OtherVector.X;
		Y += OtherVector.Y;
		Z += OtherVector.Z;
	}

	Vector operator-(Vector OtherVector)
	{
		return Vector(
			X - OtherVector.X,
			Y - OtherVector.Y,
			Z - OtherVector.Z
		);
	}

	void operator-=(Vector OtherVector)
	{
		X -= OtherVector.X;
		Y -= OtherVector.Y;
		Z -= OtherVector.Z;
	}

	XMFLOAT3 GetFormattedVector()
	{
		return XMFLOAT3(X, Y, Z);
	}
};

