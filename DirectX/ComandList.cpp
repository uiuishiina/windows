#include"ComandListclass.h"
#include<cassert>

//
CommandList::~CommandList()
{
	if (commandList_) {
		commandList_->Release();
		commandList_ = nullptr;
	}
}

//
[[nodiscard]] bool CommandList::create(const Device& devise, const ComandAllocater& Allocator)noexcept
{
	const HRESULT hr = devise.get()->CreateCommandList(0, Allocator.getType(), Allocator.get(), nullptr, IID_PPV_ARGS(&commandList_));
	if (FAILED(hr)) {
		assert(false && "コマンドリストの作成に失敗");
		return false;
	}

	commandList_->Close();
	return true;
}

//
void CommandList::reset(const ComandAllocater& Allocator)noexcept
{
	if (!commandList_) {
		assert(false && "コマンドリストが未作成です");
	}

	commandList_->Reset(Allocator.get(), nullptr);
}

//
[[nodiscard]] ID3D12GraphicsCommandList* CommandList::get()const noexcept
{
	if (commandList_) {
		assert(false && "コマンドリストが未作成");
		return nullptr;
	}
	return commandList_;
}