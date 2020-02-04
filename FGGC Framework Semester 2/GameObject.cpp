#include "GameObject.h"
#include "ParticleModel.h"

GameObject::GameObject(string type, Geometry geometry, Material material) : _type(type)
{
	_parent = nullptr;
	_transform = new Transform();
	_transform->_position = Vector();
	_transform->_rotation = Vector();
	_transform->_scale = Vector(1.0f, 1.0f, 1.0f);

	_appearance = new Appearance();
	_appearance->_geometry = geometry;
	_appearance->_material = material;
	_appearance->_textureRV = nullptr;
	_debugger = new Debugger();

	_particleModel = new ParticleModel(this);
}

GameObject::~GameObject()
{
	delete _transform;
	delete _appearance;
	delete _debugger;
}

void GameObject::Update(float t)
{
	// Calculate world matrix
	XMMATRIX scale = XMMatrixScaling(_transform->_scale.X, _transform->_scale.Y, _transform->_scale.Z);
	XMMATRIX rotation = XMMatrixRotationX(_transform->_rotation.X) * XMMatrixRotationY(_transform->_rotation.Y) * XMMatrixRotationZ(_transform->_rotation.Z);
	XMMATRIX translation = XMMatrixTranslation(_transform->_position.X, _transform->_position.Y, _transform->_position.Z);

	if (_type.find("Cube") != string::npos)
	{
		_debugger->PrintLog("Cuuuuuuube");
	}

	_particleModel->Update(t);

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
	pImmediateContext->IASetVertexBuffers(0, 1, &_appearance->_geometry.vertexBuffer, &_appearance->_geometry.vertexBufferStride, &_appearance->_geometry.vertexBufferOffset);
	pImmediateContext->IASetIndexBuffer(_appearance->_geometry.indexBuffer, DXGI_FORMAT_R16_UINT, 0);

	pImmediateContext->DrawIndexed(_appearance->_geometry.numberOfIndices, 0, 0);
}
