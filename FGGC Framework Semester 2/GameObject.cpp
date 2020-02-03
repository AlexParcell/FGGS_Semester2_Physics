#include "GameObject.h"

GameObject::GameObject(string type, Geometry geometry, Material material) : _geometry(geometry), _type(type), _material(material)
{
	_parent = nullptr;
	_position = XMFLOAT3();
	_rotation = XMFLOAT3();
	_scale = XMFLOAT3(1.0f, 1.0f, 1.0f);

	_textureRV = nullptr;
	_debugger = new Debugger();
}

GameObject::~GameObject()
{
	delete _debugger;
}

void GameObject::Update(float t)
{
	// Calculate world matrix
	XMMATRIX scale = XMMatrixScaling(_scale.X, _scale.Y, _scale.Z);
	XMMATRIX rotation = XMMatrixRotationX(_rotation.X) * XMMatrixRotationY(_rotation.Y) * XMMatrixRotationZ(_rotation.Z);
	XMMATRIX translation = XMMatrixTranslation(_position.X, _position.Y, _position.Z);

	if (_type.find("Cube") != string::npos)
	{
		_debugger->PrintLog("Cuuuuuuube");
	}

	XMStoreFloat4x4(&_world, scale * rotation * translation);

	if (_parent != nullptr)
	{
		XMStoreFloat4x4(&_world, this->GetWorldMatrix() * _parent->GetWorldMatrix());
	}
}

void GameObject::Draw(ID3D11DeviceContext * pImmediateContext)
{
	// NOTE: We are assuming that the constant buffers and all other draw setup has already taken place

	// Set vertex and index buffers
	pImmediateContext->IASetVertexBuffers(0, 1, &_geometry.vertexBuffer, &_geometry.vertexBufferStride, &_geometry.vertexBufferOffset);
	pImmediateContext->IASetIndexBuffer(_geometry.indexBuffer, DXGI_FORMAT_R16_UINT, 0);

	pImmediateContext->DrawIndexed(_geometry.numberOfIndices, 0, 0);
}
