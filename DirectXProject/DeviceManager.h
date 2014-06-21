#pragma once
#include <d3d11.h>

class DeviceManager
{
private:
	// DX11�Ŏg�p����O���[�o���ϐ�
	bool					g_InitDX11 = false;//2�d�������h�~
	HWND					g_hWnd = NULL;
	D3D_DRIVER_TYPE			g_DriverType = D3D_DRIVER_TYPE_NULL;
	D3D_FEATURE_LEVEL		FeatureLevel = D3D_FEATURE_LEVEL_11_0;
	ID3D11Device*			pDevice = NULL;
	ID3D11DeviceContext*	pDeviceContext = NULL;
	IDXGISwapChain*			g_pSwapChain = NULL;
	ID3D11RenderTargetView*	g_pRenderTargetView = NULL;
	ID3D11VertexShader* vShader;
	ID3D11PixelShader* pShader;
	ID3D11InputLayout* iLayout;
	ID3D11Buffer* pBuffer;



public:
	DeviceManager();
	HRESULT InitDX11(HWND hwnd);
	// DirectX11�I��
	void ExitDX11();
	// �����_�����O
	void RenderDX11();
};

