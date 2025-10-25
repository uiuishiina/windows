#include"DXGIclass.h"
#include<cassert>

#pragma comment(lib,"dxgi.lib")

DXGI::~DXGI()//デストラクタで解放
{
	//ファクトリー
	if (dxgifactory_){
		dxgifactory_->Release();//->でアドレス参照しながら内部の関数実行
		dxgifactory_ = nullptr;
	}
	//アダプター
	if (dxgiadapter_){
		dxgiadapter_->Release();
		dxgiadapter_ = nullptr;
	}
}

//ディスプレイの設定・・資料では分割してたが長くなるので一括
[[nodiscard]] bool DXGI::SetDisplayAdapter()noexcept
{
	//#if_DEBUG
	ID3D12Debug* debug;//デバックシステム設定
	if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debug)))){
		debug->EnableDebugLayer();
	}
	// #endif

	UINT CreateFactoryFlags = 0;
	//#if_DEBUG
	CreateFactoryFlags |= DXGI_CREATE_FACTORY_DEBUG;//or演算で返してるらしい
	// #endif

	//ファクトリー作成--------------------------------------------------------------------
	const HRESULT hr = CreateDXGIFactory2(CreateFactoryFlags, IID_PPV_ARGS(&dxgifactory_));
	if (FAILED(hr)){
		assert(false && "DXGIファクトリーの作成に失敗");
		return false;
	}
	//ファクトリーポインターどこで設定してるかわからん
	//IID_PPV_ARGS(&dxgifactory_))<-ワンちゃんここ

	//アダプター選択-------------------------------------------------------------------------
	auto Select = 0;//資料だとUINTだがautoの理由がわからん
	IDXGIAdapter1* Adapter{};

	//条件に合うのを取得するまで流す
	while (dxgifactory_->EnumAdapters1(Select, &Adapter) != DXGI_ERROR_NOT_FOUND)
	{
		Select++;
		DXGI_ADAPTER_DESC1 desc;
		Adapter->GetDesc1(&desc);

		//ソフトウエアは除外
		if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE){
			Adapter->Release();
			continue;
		}

		//Direct3D12が動かないなら除外・・FAILEDをSUCCEEDEDにすると動くならになる
		if (FAILED(D3D12CreateDevice(Adapter, D3D_FEATURE_LEVEL_11_0, _uuidof(ID3D12Device), nullptr))){
			Adapter->Release();
			continue;
		}

		dxgiadapter_ = Adapter;//条件に合うのでセット
		break;
	}

	//最後に確認
	if (!dxgiadapter_){
		assert(false && "アダプターの取得に失敗");
		return false;
	}
	return true;
}

//ファクトリーの設定確認--------------------------------------------------------
[[nodiscard]] IDXGIFactory4* DXGI::Factry()const noexcept
{
	if (!dxgifactory_){
		assert(false && "ファクトリーが未作成です");
		return nullptr;
	}
	return dxgifactory_;
}

//ディスプレイアダプターの設定確認----------------------------------------------------
[[nodiscard]] IDXGIAdapter1* DXGI::DIsplayAdapter()const noexcept
{
	if (!dxgiadapter_){
		assert(false && "アダプターが未作成です");
		return nullptr;
	}
	return dxgiadapter_;
}