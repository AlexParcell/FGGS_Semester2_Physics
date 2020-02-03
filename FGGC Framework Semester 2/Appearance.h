#pragma once
#include <d3d11_1.h>
#include <directxmath.h>

using namespace DirectX;

struct Geometry
{
	ID3D11Buffer* vertexBuffer;
	ID3D11Buffer* indexBuffer;
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

class Appearance
{
public:
	Appearance()
	{

	}

	Appearance(Material mtrl, Geometry geo)
	{
		_geometry = geo;
		_material = mtrl;
	}


	Geometry _geometry;
	Material _material;

	Geometry GetGeometryData() const { return _geometry; }

	Material GetMaterial() const { return _material; }

	ID3D11ShaderResourceView* _textureRV;

	void SetTextureRV(ID3D11ShaderResourceView* textureRV) { _textureRV = textureRV; }
	ID3D11ShaderResourceView* GetTextureRV() const { return _textureRV; }
	bool HasTexture() const { return _textureRV ? true : false; }

};

