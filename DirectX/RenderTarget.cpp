#include"ReaderTargetclass.h"
#include<cassert>

//レンダーターゲットの配列の解放
RenderTarget:: ~RenderTarget()
{
	for (auto& rt : renderTargets_) {
		if (rt) {
			rt->Release();
			rt = nullptr;
		}
	}
	renderTargets_.clear();
}

//バッファの作成
[[nodiscard]] bool RenderTarget::create(const Device& device, const Swapchain& swap, const DescripterHeap& heap)noexcept
{
	const auto& desc = swap.getDesc();
	renderTargets_.resize(desc.BufferCount);
	auto handle = heap.get()->GetCPUDescriptorHandleForHeapStart();
	auto heapType = heap.getType();
	assert(heapType == D3D12_DESCRIPTOR_HEAP_TYPE_RTV && "ディスクリプターヒープのタイプが一致してません");

	for (uint8_t i = 0; i < desc.BufferCount; i++) {
		const HRESULT hr = swap.get()->GetBuffer(i, IID_PPV_ARGS(&renderTargets_[i]));
		if (FAILED(hr)) {
			assert(false && "バックバッファの取得に失敗");
			return false;
		}

		device.get()->CreateRenderTargetView(renderTargets_[i], nullptr, handle);

		handle.ptr += device.get()->GetDescriptorHandleIncrementSize(heapType);
	}

	return true;
}

//ディスクリプターハンドルを返す
[[nodiscard]] D3D12_CPU_DESCRIPTOR_HANDLE RenderTarget::getDescripterHandle(const Device& device, const DescripterHeap& heap, UINT index)const noexcept
{
	if (index >= renderTargets_.size() || !renderTargets_[index]) {
		assert(false && "不正なレンダーターゲット");
	}

	auto handle = heap.get()->GetCPUDescriptorHandleForHeapStart();
	
	auto heapType = heap.getType();
	assert(heapType == D3D12_DESCRIPTOR_HEAP_TYPE_RTV && "ディスクリプターヒープのタイプが一致してません");

	handle.ptr += device.get()->GetDescriptorHandleIncrementSize(heapType);
	return handle;
}

//レンダーターゲットのポインターを返す
[[nodiscard]] ID3D12Resource* RenderTarget::get(uint32_t index)const noexcept
{
	if (index >= renderTargets_.size()) {
		assert(false && "レンダーターゲットのサイズが配列の範囲外です");
		return nullptr;
	}

	if (!renderTargets_[index]) {
		assert(false && "レンダーターゲットが確保されていません");
		return nullptr;
	}
	return renderTargets_[index];
}