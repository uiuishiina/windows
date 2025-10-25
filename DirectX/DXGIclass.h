#pragma once
//DXGIのクラス・・includeするなら/.h/のほうがいいらしい
#include<d3d12.h>
#include<dxgi1_4.h>

class DXGI//DirectXに必要なデバイスの作成に必要なポインタの取得・・GPU制御に必要なインスタンスの確保
{
public:
	DXGI() = default;
	~DXGI();//ポインターの初期化

	//ディスプレイの設定・・trueなら取得できている
	[[nodiscard]] bool SetDisplayAdapter()noexcept;

	//ファクトリーの設定確認
	[[nodiscard]] IDXGIFactory4* Factry()const noexcept;

	//ディスプレイアダプターの設定確認
	[[nodiscard]] IDXGIAdapter1* DIsplayAdapter()const noexcept;
private:
	IDXGIFactory4* dxgifactory_{};//DXGI作成のファクトリー
	IDXGIAdapter1* dxgiadapter_{};//ディスプレイ設定で使うアダプター
};
