#pragma once
#include"Deviceclass.h"

class DescripterHeap
{
public:
	DescripterHeap() = default;

	//デストラクタ
	~DescripterHeap();

	//ディスクリプターヒープ作成
	[[nodiscard]] bool create(const Device& device, D3D12_DESCRIPTOR_HEAP_TYPE type, UINT numDescriptors, bool shaderVisible = false)noexcept;

	//ディスクリプターヒープ取得
	[[nodiscard]] ID3D12DescriptorHeap* get()const noexcept;

	//ディスクリプターヒープのタイプを取得
	[[nodiscard]] D3D12_DESCRIPTOR_HEAP_TYPE getType()const noexcept;
private:
	ID3D12DescriptorHeap* heap_{};      //作ったディスクリプターヒープのアドレス
	D3D12_DESCRIPTOR_HEAP_TYPE type_{}; //ディスクリプターヒープのタイプを保存
};
