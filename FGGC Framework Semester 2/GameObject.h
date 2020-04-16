#pragma once

#include <directxmath.h>
#include <d3d11_1.h>
#include <string>
#include <vector>

using namespace std;

#include "Debugger.h"
#include "Appearance.h"
#include "Transform.h"

class ParticleModel;

using namespace DirectX;
using namespace std;

class GameObject
{
public:
	GameObject(string type, Geometry geometry, Material material, Vector boundingBox);
	~GameObject();

	string GetType() const { return _type; }

	XMMATRIX GetWorldMatrix() const { return XMLoadFloat4x4(&_world); }

	Transform* GetTransform() { return _transform; }
	Appearance* GetAppearance() { return _appearance; }
	ParticleModel* GetParticleModel() { return _particleModel; }

	Vector GetBoundingBox() { return BoundingBox; }

	void SetParent(GameObject * parent) { _parent = parent; }


	void Update(float t);
	void Draw(ID3D11DeviceContext * pImmediateContext);

private:
	Transform* _transform = nullptr;
	Appearance* _appearance = nullptr;
	ParticleModel* _particleModel = nullptr;

	Vector BoundingBox;

	string _type;

	XMFLOAT4X4 _world;

	GameObject * _parent;

	Debugger* _debugger;
};

