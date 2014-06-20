#include "stdafx.h"
#include "DeviceManager.h"


DeviceManager::DeviceManager()
{
	HRESULT hr = S_OK;
	UINT cdev_flags = 0;
#ifdef _DEBUG
	cdev_flags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
	
	D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL,
		cdev_flags,&featureLevel, 1, D3D11_SDK_VERSION,&sd,
		&g_pSwapChain, &pDevice, &featureLevel, &pDeviceContext);

	// スワップチェインに用意されたバッファ（2Dテクスチャ）を取得
	ID3D11Texture2D* back_buff = NULL;
	hr = g_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&back_buff);
	

	hr = pDevice->CreateRenderTargetView(back_buff, NULL, &g_pRenderTargetView);
	back_buff->Release();//GetBufferで取得、使用後はRelease


	// render-target viewを登録
	pDeviceContext->OMSetRenderTargets(1, &g_pRenderTargetView, NULL);
	//これでレンダリング結果がスワップチェインのバッファに書き込まれる


	// 指定色で画面クリア
	float ClearColor[4] = { 0.0f, 0.125f, 0.3f, 1.0f }; //red,green,blue,alpha
	pDeviceContext->ClearRenderTargetView(g_pRenderTargetView, ClearColor);

	//結果をウインドウに反映
	g_pSwapChain->Present(0, 0);


}


DeviceManager::~DeviceManager()
{
}

void DeviceManager::OnDispose()
{

	if (g_pRenderTargetView) g_pRenderTargetView->Release();
	if (pDevice != NULL)pDevice->Release();
	if (pDeviceContext != NULL)pDeviceContext->Release();
}
