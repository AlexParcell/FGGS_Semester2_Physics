#pragma once

#include <directxmath.h>
#include <d3d11_1.h>
#include <string>

#include "Debugger.h"
#include "Vector.h"

using namespace DirectX;
using namespace std;

struct Geometry
{
	ID3D11Buffer * vertexBuffer;
	ID3D11Buffer * indexBuffer;
	int numberOfIndices;

	UINT vertexBufferStride;
	UINT vertexBufferOffset;
};

struct Material
{
	XMFLOAT4 diffuse;
	XMFLOAT4 ambient;
	XMFLOAT4 specular;
	float specularPower;
};

class GameObject
{
public:
	GameObject(string type, Geometry geometry, Material material);
	~GameObject();

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

	string GetType() const { return _type; }

	Geometry GetGeometryData() const { return _geometry; }

	Material GetMaterial() const { return _material; }

	XMMATRIX GetWorldMatrix() const { return XMLoadFloat4x4(&_world); }

	void SetTextureRV(ID3D11ShaderResourceView * textureRV) { _textureRV = textureRV; }
	ID3D11ShaderResourceView * GetTextureRV() const { return _textureRV; }
	bool HasTexture() const { return _textureRV ? true : false; }

	void SetParent(GameObject * parent) { _parent = parent; }

	void Update(float t);
	void Draw(ID3D11DeviceContext * pImmediateContext);

private:
	Vector _scale;
	Vector _rotation;
	Vector _position;

	string _type;

	XMFLOAT4X4 _world;

	Geometry _geometry;
	Material _material;

	ID3D11ShaderResourceView * _textureRV;

	GameObject * _parent;

	Debugger* _debugger;
};

