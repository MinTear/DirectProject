#pragma once
#include <d3d11.h>
class DrawObject
{
private:
	ID3D11VertexShader* vShader;
	ID3D11PixelShader* pShader;
	ID3D11InputLayout* iLayout;
	ID3D11Buffer* pBuffer;


public:
	DrawObject();
};

