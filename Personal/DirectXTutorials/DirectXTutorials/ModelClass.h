
// ModelClass�� 3D���� ���ϱ����� ĸ��ȭ�ϴ� Ŭ����
// �ﰢ���� ����� ���� �����Ϳ� ��������, �ε������۸� ������.

#pragma once


#include <d3d11.h>
#include <DirectXMath.h>

#include "TextureClass.h"


class ModelClass
{
private:

	// ���ؽ� ���ۿ� ����� ����ü�� ����. ���ؽ� ���̴��� ����ü�� ������ ���ƾ� ��.
	struct VertexType
	{
		DirectX::XMFLOAT3 Position;
		DirectX::XMFLOAT2 Texture;
		DirectX::XMFLOAT3 Normal;
	};

public:

	ModelClass();
	ModelClass(const ModelClass&);
	~ModelClass();

	bool Initialize(ID3D11Device*, ID3D11DeviceContext*, const char*); // �ʱ�ȭ
	void Shutdown(); // ����
	void Render(ID3D11DeviceContext*); 

	int GetIndexCount() const;
	ID3D11ShaderResourceView* GetTexture() const;

private:

	bool InitializeBuffers(ID3D11Device*);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext*);
	bool LoadTexture(ID3D11Device*, ID3D11DeviceContext*, const char*);
	void ReleaseTexture();
	
private:

	// ���� ������ ���� ������ Description���� ������.
	ID3D11Buffer* mVertexBuffer;
	ID3D11Buffer* mIndexBuffer;
	TextureClass* mTexture;
	int mVertexCount;
	int mIndexCount;

};

