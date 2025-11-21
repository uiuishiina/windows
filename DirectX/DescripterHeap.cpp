#include"DescripterHeapclass.h"
#include<cassert>;

//メモリの解放
DescripterHeap::~DescripterHeap()
{
	if (heap_) {
		heap_->Release();
		heap_ = nullptr;
	}
}

//ディスクリプターヒープ作成
[[nodiscard]] bool DescripterHeap::create(const Device& device, D3D12_DESCRIPTOR_HEAP_TYPE type, UINT numDescriptors, bool shaderVisible )noexcept
{
	D3D12_DESCRIPTOR_HEAP_DESC heapdesc = {};
	heapdesc.Type = type;
	heapdesc.NumDescriptors = numDescriptors;
	heapdesc.Flags = shaderVisible ? D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE : D3D12_DESCRIPTOR_HEAP_FLAG_NONE;

	HRESULT hr = device.get()->CreateDescriptorHeap(&heapdesc, IID_PPV_ARGS(&heap_));
	if (FAILED(hr)) {
		assert(false && "ディスクリプターヒープの作成に失敗");
		return false;
	}

	type_ = type;
	return true;
}


//ディスクリプターヒープのポインターを返す
[[nodiscard]] ID3D12DescriptorHeap* DescripterHeap::get()const noexcept
{
	if (!heap_) {
		assert(false && "ディスクリプターヒープが未作成");
		//できてないならNullを返す
		return nullptr;
	}
	//ヒープができているならポインターを返す
	return heap_;
}

//ディスクリプターヒープのタイプを返す
[[nodiscard]] D3D12_DESCRIPTOR_HEAP_TYPE DescripterHeap::getType()const noexcept
{
	if (!heap_) {
		assert(false && "ディスクリプターヒープが未作成のためタイプ未設定");
	}
	return type_;
}