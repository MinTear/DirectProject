#pragma once
#include "Disposable.h"
#include <d3d11.h>
class DeviceManager :
	public Disposable {
private:
	ID3D11Device* pDevice;
	ID3D11DeviceContext* pDeviceContext;
	D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL_11_0;
	DXGI_SWAP_CHAIN_DESC sd;
	ID3D11RenderTargetView*	g_pRenderTargetView = NULL;
	IDXGISwapChain*	g_pSwapChain = NULL;
public:
	DeviceManager();
	~DeviceManager();
	void OnDispose();
};

