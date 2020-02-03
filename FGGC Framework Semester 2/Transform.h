#pragma once
#include "Vector.h"

class Transform
{

public:
	Vector _position;
	Vector _scale;
	Vector _rotation;

	Transform()
	{

	}

	Transform(Vector Position, Vector Scale, Vector Rotation)
	{
		_position = Position;
		_scale = Scale;
		_rotation = Rotation;
	}

	// Setters and Getters for position/rotation/scale
	void SetPosition(Vector position) { _position = position; }
	void SetPosition(float x, float y, float z) { _position.X = x; _position.Y = y; _position.Z = z; }

	Vector GetPosition() const { return _position; }

	void SetScale(Vector scale) { _scale = scale; }
	void SetScale(float x, float y, float z) { _scale.X = x; _scale.Y = y; _scale.Z = z; }

	Vector GetScale() const { return _scale; }

	void SetRotation(Vector rotation) { _rotation = rotation; }
	void SetRotation(float x, float y, float z) { _rotation.X = x; _rotation.Y = y; _rotation.Z = z; }

	Vector GetRotation() const { return _rotation; }
};

