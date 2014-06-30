#include "stdafx.h"
#include "DeviceManager.h"
#include <d3d11.h>
#include <D3DX11.h>
#include<directxmath.h>
#include<d3dcompiler.h>

using namespace DirectX;

struct VertexData
{
	XMFLOAT4 Pos;
	XMFLOAT2 Tex;
};

DeviceManager::DeviceManager()
{
}

	// シェーダのコンパイル
HRESULT CompileShaderFromFile(WCHAR* fname, LPCSTR entry_point, LPCSTR shd_model, ID3DBlob** pp_blob_out)
{
		HRESULT hr = S_OK;

		DWORD shd_flags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined( DEBUG ) || defined( _DEBUG )
		shd_flags |= D3DCOMPILE_DEBUG;
#endif

		ID3DBlob* err_blob;
		hr = D3DCompileFromFile(fname, NULL, NULL, entry_point, shd_model,
			shd_flags, 0, pp_blob_out, &err_blob);
		if (FAILED(hr)){
			if (err_blob != NULL){
				OutputDebugStringA((char*)err_blob->GetBufferPointer());
			}
			if (err_blob) err_blob->Release();
			return hr;
		}
		if (err_blob){
			err_blob->Release();
		}
		return S_OK;
}


HRESULT DeviceManager::InitDX11(HWND hwnd)
{
	if (g_InitDX11)return S_FALSE;//すでに初期化
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

	// DirectX11&ハードウェア対応のみ
	D3D_FEATURE_LEVEL feature_level = D3D_FEATURE_LEVEL_11_0;
	g_DriverType = D3D_DRIVER_TYPE_HARDWARE;

	// スワップチェイン設定
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

	// DirectX11デバイスとスワップチェイン作成
	hr = D3D11CreateDeviceAndSwapChain(NULL, g_DriverType, NULL,
		cdev_flags, &feature_level, 1, D3D11_SDK_VERSION, &sd,
		&g_pSwapChain, &pDevice, &FeatureLevel, &pDeviceContext);
	if (FAILED(hr)){
		return hr;
	}

	// render-target viewの作成
	// ポリゴンなどのレンダリング結果の出力場所を設定、管理（ID3D11RenderTargetView)
	// このプログラムでは、スワップチェインに用意された2Dテクスチャを
	// レンダリング対象に設定する

	// スワップチェインに用意されたバッファ（2Dテクスチャ）を取得
	ID3D11Texture2D* back_buff = NULL;
	hr = g_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&back_buff);
	if (FAILED(hr)){
		return hr;
	}

	D3D11_TEXTURE2D_DESC descDepth;
	ZeroMemory(&descDepth, sizeof(descDepth));
	descDepth.Width = width;
	descDepth.Height = height;
	descDepth.MipLevels = 1;
	descDepth.ArraySize = 1;
	descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	descDepth.SampleDesc.Count = 1;
	descDepth.SampleDesc.Quality = 0;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	descDepth.CPUAccessFlags = 0;
	descDepth.MiscFlags = 0;
	hr = pDevice->CreateTexture2D(&descDepth, NULL, &g_pDepthStencil);
	if (FAILED(hr))
		return hr;

	hr = pDevice->CreateRenderTargetView(back_buff, NULL, &g_pRenderTargetView);
	back_buff->Release();//GetBufferで取得、使用後はRelease
	if (FAILED(hr)){
		return hr;
	}

	// render-target viewを登録
	pDeviceContext->OMSetRenderTargets(1, &g_pRenderTargetView, NULL);
	//これでレンダリング結果がスワップチェインのバッファに書き込まれる

	// viewport
	D3D11_VIEWPORT vp;
	vp.Width = (FLOAT)width;
	vp.Height = (FLOAT)height;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	pDeviceContext->RSSetViewports(1, &vp);

	// Compile the vertex shader
	ID3DBlob* pVSBlob = NULL;

	hr = CompileShaderFromFile(L"Tutorial02.fx", "VS", "vs_4_0", &pVSBlob);
	if (FAILED(hr))
	{
		MessageBox(NULL,
			L"The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", L"Error", MB_OK);
		return hr;
	}

	// Create the vertex shader
	hr = pDevice->CreateVertexShader(pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), NULL, &vShader);
	if (FAILED(hr))
	{
		pVSBlob->Release();
		return hr;
	}

	// Define the input layout
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 16, D3D11_INPUT_PER_VERTEX_DATA, 0 },

	};
	UINT numElements = ARRAYSIZE(layout);

	// Create the input layout
	hr = pDevice->CreateInputLayout(layout, numElements, pVSBlob->GetBufferPointer(),
		pVSBlob->GetBufferSize(), &iLayout);
	pVSBlob->Release();
	if (FAILED(hr))
		return hr;

	// Set the input layout
	pDeviceContext->IASetInputLayout(iLayout);

	// Compile the pixel shader
	ID3DBlob* pPSBlob = NULL;


	hr = CompileShaderFromFile(L"Tutorial02.fx", "PS", "ps_4_0", &pPSBlob);
	if (FAILED(hr))
	{
		MessageBox(NULL,
			L"The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", L"Error", MB_OK);
		return hr;
	}

	// Create the pixel shader
	hr = pDevice->CreatePixelShader(pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), NULL, &pShader);
	pPSBlob->Release();
	if (FAILED(hr))
		return hr;

	// Create vertex buffer
	VertexData vertices[] =
	{
		{ XMFLOAT4(-0.800f, 0.581f, 0.5f, 1.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT4(0.800f, 0.581f, 0.5f, 1.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT4(0.800f, -0.581f, 0.5f, 1.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT4(0.800f, -0.581f, 0.5f, 1.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT4(-0.800f, -0.581f, 0.5f, 1.0f), XMFLOAT2(0.0f, 1.0f) },
		{ XMFLOAT4(-0.800f, 0.581f, 0.5f, 1.0f), XMFLOAT2(0.0f, 0.0f) },

	};

	settingBuffer(hr, vertices, pDevice, pDeviceContext, pBuffer);

	hr = D3DX11CreateShaderResourceViewFromFile(pDevice, L"BuildingsHighRise0295_2_M.jpg", NULL, NULL, &shaderRV, NULL);
	if (FAILED(hr))
		return hr;

	// Create the sample state
	D3D11_SAMPLER_DESC sampDesc;
	ZeroMemory(&sampDesc, sizeof(sampDesc));
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampDesc.MinLOD = 0;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
	hr = pDevice->CreateSamplerState(&sampDesc, &SamplerLinear);
	if (FAILED(hr))
		return hr;
	
	return S_OK;
}

void DeviceManager::ExitDX11()
{

	if (!g_InitDX11)return;//初期化してない

	if (pDeviceContext) pDeviceContext->ClearState();

	if (SamplerLinear) SamplerLinear->Release();
	if (shaderRV) shaderRV->Release();
	if (pBuffer) pBuffer->Release();
	if (iLayout) iLayout->Release();
	if (vShader) vShader->Release();
	if (pShader) pShader->Release();
	if (g_pRenderTargetView) g_pRenderTargetView->Release();
	if (g_pSwapChain) g_pSwapChain->Release();
	if (pDeviceContext) pDeviceContext->Release();
	if (pDevice) pDevice->Release();

	g_InitDX11 = false;
}


void DeviceManager::RenderDX11()
{
	// 指定色で画面クリア
	float ClearColor[4] = { 0.0f, 0.125f, 0.3f, 1.0f }; //red,green,blue,alpha
	pDeviceContext->ClearRenderTargetView(g_pRenderTargetView, ClearColor);

	// Render
	pDeviceContext->VSSetShader(vShader, NULL, 0);
	pDeviceContext->PSSetShader(pShader, NULL, 0);
	pDeviceContext->PSSetSamplers(0, 1, &SamplerLinear);	
	pDeviceContext->PSSetShaderResources(0, 1, &shaderRV);
	pDeviceContext->Draw(6, 0);

	//結果をウインドウに反映
	g_pSwapChain->Present(0, 0);
}