#pragma once
//デバイスのクラス

#include"DXGIclass.h"
#include<d3d12.h>

class Device
{
public:
	Device() = default;
	~Device();//ポインター初期化

	//デバイス作成・・trueなら取得できている
	[[nodiscard]] bool Create(const DXGI& dxgi)noexcept;

	//デバイス取得
	[[nodiscard]] ID3D12Device* get()const noexcept;

private:
	ID3D12Device* device_;
};
