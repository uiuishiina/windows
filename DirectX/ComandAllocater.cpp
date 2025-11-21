#include"ComandAllocaterclass.h"
#include<cassert>


//アドレスを解放
ComandAllocater::~ComandAllocater()
{
	if (commandAllocater_) {
		commandAllocater_->Release();
		commandAllocater_ = nullptr;
	}
}

//コマンドアロケーターの作成
[[nodiscard]] bool ComandAllocater::create(const Device& device, const D3D12_COMMAND_LIST_TYPE type) noexcept
{
	HRESULT hr = device.get()->CreateCommandAllocator(type_, IID_PPV_ARGS(&commandAllocater_));
	if (FAILED(hr))
	{
		assert(false && "コマンドアロケーターの作成に失敗");
		return false;
	}

	type_ = type;
	return true;
}

//コマンドアロケーターリセット
void ComandAllocater::reset()noexcept
{
	if (!commandAllocater_) {
		assert(false && "コマンドアロケーター未作成");
	}
	//できてるならリセット
	commandAllocater_->Reset();
}

//コマンドアロケーターのポインターを返す
[[nodiscard]] ID3D12CommandAllocator* ComandAllocater::get()const noexcept
{
	if (!commandAllocater_) {
		assert(false && "コマンドアロケーター未作成");
		return nullptr;
	}
	return commandAllocater_;
}

//コマンドアロケーターのタイプを返す
[[nodiscard]] D3D12_COMMAND_LIST_TYPE ComandAllocater::getType()const noexcept
{
	if (!commandAllocater_) {
		assert(false && "コマンドアロケーター未作成のためタイプ未設定");
	}
	return type_;
}