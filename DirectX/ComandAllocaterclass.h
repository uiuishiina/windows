#pragma once

#include"Deviceclass.h"

class ComandAllocater
{
public:
	ComandAllocater() = default;

	//デストラクタ
	~ComandAllocater();

	//コマンドアロケーターの作成
	[[nodiscard]] bool create(const Device& device, const D3D12_COMMAND_LIST_TYPE type) noexcept;

	//コマンドアロケーターのリセット
	void reset()noexcept;

	//コマンドアロケーターのポインターを取得
	[[nodiscard]] ID3D12CommandAllocator* get()const noexcept;

	//コマンドアロケーターのタイプを取得
	[[nodiscard]] D3D12_COMMAND_LIST_TYPE getType()const noexcept;
private:
	ID3D12CommandAllocator* commandAllocater_{};  //コマンドアロケーターのアドレス
	D3D12_COMMAND_LIST_TYPE type_{};              //コマンドアロケーターのタイプ
};
