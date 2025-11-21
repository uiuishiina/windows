#pragma once
#include"deviceaccess.h"
#include"Swapchainclassh.h"
#include"DescripterHeapclass.h"
#include<vector>


class RenderTarget
{
public:
	RenderTarget() = default;

	//デストラクタ
	~RenderTarget();

	//レンダーターゲット作成
	[[nodiscard]] bool create(const Device& device, const Swapchain& swap, const DescripterHeap& heap)noexcept;

	//ハンドルの取得
	[[nodiscard]] D3D12_CPU_DESCRIPTOR_HANDLE getDescripterHandle(const Device& device, const DescripterHeap& heap, UINT index)const noexcept;

	//レンダーターゲットの取得
	[[nodiscard]] ID3D12Resource* get(uint32_t index)const noexcept;
private:
	std::vector<ID3D12Resource*> renderTargets_;
};