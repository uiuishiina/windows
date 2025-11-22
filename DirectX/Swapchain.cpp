#include"Swapchainclassh.h"
#include<assert.h>


Swapchain::~Swapchain()
{
	if (swapchain_){
		swapchain_->Release();
		swapchain_ = nullptr;
	}
}

[[nodiscard]] bool Swapchain::create(const DXGI& dxgi, const CommandQueue& command, const window& wind)noexcept
{
	const auto[w, h] = wind.size();
	DXGI_SWAP_CHAIN_DESC1 desc = {};
	desc.BufferCount = 2;
	desc.Width = w;
	desc.Height = h;
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	desc.SampleDesc.Count = 1;

	swapDesc_ = desc;
	IDXGISwapChain1* tempSwapChain{};{
		const HRESULT hr = dxgi.Factry()->CreateSwapChainForHwnd(command.get(), wind.handle(), &desc, nullptr, nullptr, &tempSwapChain);
		if (FAILED(hr)){
			assert(false && "スワップチェインの作成に失敗");
			return false;
		}
	}
	
	{
		const HRESULT hr = tempSwapChain->QueryInterface(IID_PPV_ARGS(&swapchain_));
		tempSwapChain->Release();//初期化する必要はないのか

		if (FAILED(hr)){
			assert(false && "スワップチェインのキャストに失敗");
			return false;
		}
	}
	return true;
}

[[nodiscard]] IDXGISwapChain3* Swapchain::get()const noexcept
{
	if (!swapchain_){
		assert(false && "スワップチェインの取得に失敗");
		return nullptr;
	}
	return swapchain_;
}

[[nodiscard]] const DXGI_SWAP_CHAIN_DESC1& Swapchain::getDesc()const noexcept
{
	if (!swapchain_) {
		assert(false && "スワップチェーンを未作成");
	}
	return swapDesc_;
}