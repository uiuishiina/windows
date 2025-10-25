#pragma once
//コマンドキュークラス

#include"Deviceclass.h"

class CommandQueue
{
public:
	CommandQueue() = default;
	~CommandQueue();//ポインター初期化

	//コマンドキュー作成
	[[nodiscard]] bool create(const Device& device)noexcept;

	//コマンドキュー取得
	[[nodiscard]] ID3D12CommandQueue* get()const noexcept;
private:
	ID3D12CommandQueue* commandQueue_;
};
