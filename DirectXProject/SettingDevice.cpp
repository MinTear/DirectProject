#include "stdafx.h"
#include "SettingDevice.h"
#include <d3dcompiler.h>

SettingDevice::SettingDevice()
{
}


void SettingDevice::settingBuffer(HRESULT hr,VertexData *vData,ID3D11Device* pDevice,ID3D11DeviceContext* pDeviceContext,ID3D11Buffer* pBuffer) 
{
	//VertexBuffer‚ðì¬
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(VertexData)*6;
	bd.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory(&InitData, sizeof(InitData));
	InitData.pSysMem = vData;

	hr = pDevice->CreateBuffer(&bd, &InitData, &pBuffer);
	if (FAILED(hr))
		return;

	//VertxBuffer‚ÌÝ’è
	UINT stride = sizeof(VertexData);
	UINT offset = 0;
	pDeviceContext->IASetVertexBuffers(0, 1, &pBuffer, &stride, &offset);

	pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);



}