#include "stdafx.h"
#include "DeviceManager.h"


DeviceManager::DeviceManager()
{
}

HRESULT DeviceManager::InitDX11(HWND hwnd)
{
	if (g_InitDX11)return S_FALSE;//���łɏ�����
	g_hWnd = hwnd;

	HRESULT hr = S_OK;

	RECT rc;
	GetClientRect(g_hWnd, &rc);
	UINT width = rc.right - rc.left;
	UINT height = rc.bottom - rc.top;

	UINT cdev_flags = 0;
#ifdef _DEBUG
	cdev_flags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	// DirectX11&�n�[�h�E�F�A�Ή��̂�
	D3D_FEATURE_LEVEL feature_level = D3D_FEATURE_LEVEL_11_0;
	g_DriverType = D3D_DRIVER_TYPE_HARDWARE;

	// �X���b�v�`�F�C���ݒ�
	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.BufferCount = 1;
	sd.BufferDesc.Width = width;
	sd.BufferDesc.Height = height;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;	//1/60 = 60fps
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = g_hWnd;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.Windowed = TRUE;

	// DirectX11�f�o�C�X�ƃX���b�v�`�F�C���쐬
	hr = D3D11CreateDeviceAndSwapChain(NULL, g_DriverType, NULL,
		cdev_flags, &feature_level, 1, D3D11_SDK_VERSION, &sd,
		&g_pSwapChain, &pDevice, &FeatureLevel, &pDeviceContext);
	if (FAILED(hr)){
		return hr;
	}

	// render-target view�̍쐬
	// �|���S���Ȃǂ̃����_�����O���ʂ̏o�͏ꏊ��ݒ�A�Ǘ��iID3D11RenderTargetView)
	// ���̃v���O�����ł́A�X���b�v�`�F�C���ɗp�ӂ��ꂽ2D�e�N�X�`����
	// �����_�����O�Ώۂɐݒ肷��

	// �X���b�v�`�F�C���ɗp�ӂ��ꂽ�o�b�t�@�i2D�e�N�X�`���j���擾
	ID3D11Texture2D* back_buff = NULL;
	hr = g_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&back_buff);
	if (FAILED(hr)){
		return hr;
	}

	hr = pDevice->CreateRenderTargetView(back_buff, NULL, &g_pRenderTargetView);
	back_buff->Release();//GetBuffer�Ŏ擾�A�g�p���Release
	if (FAILED(hr)){
		return hr;
	}

	// render-target view��o�^
	pDeviceContext->OMSetRenderTargets(1, &g_pRenderTargetView, NULL);
	//����Ń����_�����O���ʂ��X���b�v�`�F�C���̃o�b�t�@�ɏ������܂��

	// viewport
	D3D11_VIEWPORT vp;
	vp.Width = (FLOAT)width;
	vp.Height = (FLOAT)height;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	pDeviceContext->RSSetViewports(1, &vp);

	g_InitDX11 = true;
	return S_OK;
}

void DeviceManager::ExitDX11()
{

	if (!g_InitDX11)return;//���������ĂȂ�

	if (pDeviceContext) pDeviceContext->ClearState();

	if (g_pRenderTargetView) g_pRenderTargetView->Release();
	if (g_pSwapChain) g_pSwapChain->Release();
	if (pDeviceContext) pDeviceContext->Release();
	if (pDevice) pDevice->Release();

	g_InitDX11 = false;
}


void DeviceManager::RenderDX11()
{
	// �w��F�ŉ�ʃN���A
	float ClearColor[4] = { 0.0f, 0.125f, 0.3f, 1.0f }; //red,green,blue,alpha
	pDeviceContext->ClearRenderTargetView(g_pRenderTargetView, ClearColor);

	//���ʂ��E�C���h�E�ɔ��f
	g_pSwapChain->Present(0, 0);
}