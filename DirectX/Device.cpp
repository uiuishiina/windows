#include"Deviceclass.h"
#include<cassert>

#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxguid.lib")

//デストラクタ
Device::~Device()
{
	if (device_){
		device_->Release();
		device_ = nullptr;
	}
}

//デバイス作成
[[nodiscard]] bool Device::Create(const DXGI& dxgi)noexcept
{
	//DXGIclassの関数でポインタ取得
	const HRESULT hr = D3D12CreateDevice(dxgi.DIsplayAdapter(), D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&device_));
	if (FAILED(hr)){
		assert(false && "デバイス作成に失敗");
		return false;
	}
	return true;
}

//デバイス取得
[[nodiscard]] ID3D12Device* Device::get()const noexcept
{
	if (!device_){
		assert(false && "デバイスが未作成です");
		return nullptr;
	}
	return device_;
}