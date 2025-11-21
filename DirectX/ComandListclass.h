#pragma once
#include"Deviceclass.h"
#include"ComandAllocaterclass.h"

class CommandList
{
public:
	CommandList() = default;

	//デストラクタ
	~CommandList();

	//コマンドリスト作成
	[[nodiscard]] bool create(const Device& devise, const ComandAllocater& Allocator)noexcept;

	//コマンドリストリセット
	void reset(const ComandAllocater& Allocator)noexcept;

	//コマンドリスト取得
	[[nodiscard]] ID3D12GraphicsCommandList* get()const noexcept;
private:
	ID3D12GraphicsCommandList* commandList_{};//コマンドリストのポインター
};