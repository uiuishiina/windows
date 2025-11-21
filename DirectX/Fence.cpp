#include"Fenceclass.h"
#include<cassert>

Fence::~Fence()
{
	if (fence_) {
		fence_->Release();
		fence_ = nullptr;
	}
}

[[nodiscard]] bool Fence::create(const Device& device)noexcept
{
	HRESULT hr = device.get()->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence_));
	if (FAILED(hr)) {
		assert(false && "フェンスの作成に失敗");
		return false;
	}

	waitGPUEvent_ = CreateEvent(nullptr, false, false, "WAIT_GPU");
	if (!waitGPUEvent_) {
		assert(false && "GPU用のイベントハンドルの作成に失敗");
		return false;
	}
	return true;
}

//同期用関数
void Fence::wait(UINT64 fenceValue)const noexcept
{
	if (!fence_) {
		assert(false && "フェンスが未作成");
		return;
	}

	//ここで引っかかると終わるまで待つイベントが起動する
	if (fence_->GetCompletedValue() < fenceValue) {
		//フェンスの操作が終了した番号が次操作したいスワップチェーンに振り分けた番号より小さい
		//つまり次操作したいスワップチェーンの番号が終わったときまで
		fence_->SetEventOnCompletion(fenceValue, waitGPUEvent_);
		WaitForSingleObject(waitGPUEvent_, INFINITE);
	}
}

//フェンスのポインターを返す
[[nodiscard]] ID3D12Fence* Fence::get()const noexcept
{
	if (!fence_) {
		assert(false && "フェンスが未作成");
		return nullptr;
	}
	return fence_;
}