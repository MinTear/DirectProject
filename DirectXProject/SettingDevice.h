#pragma once
#include <d3d11.h>
#include <directxmath.h>
using namespace DirectX;

class SettingDevice
{
public:
	struct VertexData
	{
		XMFLOAT4 pos;
	};


public:
	SettingDevice();
	void settingBuffer(HRESULT hr,struct VertexData *vData,ID3D11Device* pDevice,ID3D11DeviceContext* pDeviceContext,ID3D11Buffer *pBuffer);
};

