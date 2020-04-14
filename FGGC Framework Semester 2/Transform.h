#pragma once
#include "Vector.h"

class Transform
{

public:
	Vector _position = Vector(0, 0, 0);
	Vector _scale;
	Vector _rotation;
	Vector _oldPosition;

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
	void SetPosition(Vector position) { _oldPosition = _position, _position = position;}

	Vector GetPosition() const { return _position; }

	void SetScale(Vector scale) { _scale = scale; }

	Vector GetScale() const { return _scale; }

	void SetRotation(Vector rotation) { _rotation = rotation; }

	void RevertPosition() { _position = _oldPosition; }

	Vector GetRotation() const { return _rotation; }
};

