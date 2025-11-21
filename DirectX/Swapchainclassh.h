#pragma once

#include"DXGIclass.h"
#include"CommandQueueclass.h"
#include"../window/windowclass.h"

class Swapchain
{
public:
	Swapchain() = default;
	~Swapchain();
	//
	[[nodiscard]] bool create(const DXGI& dxgi, const CommandQueue& command, const window& wind)noexcept;

	//
	[[nodiscard]] IDXGISwapChain3* get()const noexcept;

	//
	[[nodiscard]] const DXGI_SWAP_CHAIN_DESC1& getDesc()const noexcept;
private:
	IDXGISwapChain3* swapchain_;
	DXGI_SWAP_CHAIN_DESC1 swapDesc_;
};