#pragma
#include"Deviceclass.h"

class Fence
{
public:
	Fence() = default;

	//デストラクタ
	~Fence();

	//フェンスを作成
	[[nodiscard]] bool create(const Device& device)noexcept;

	//同期待ちのための関数
	void wait(UINT64 fenceValue)const noexcept;

	//フェンスのポインターを取得
	[[nodiscard]] ID3D12Fence* get()const noexcept;
private:
	ID3D12Fence* fence_{};  //フェンス
	HANDLE waitGPUEvent_{}; //CPU と GPUの同期用ハンドル
};