#include"../window/windowclass.h"

#include"../DirectX/DXGIclass.h"
#include"../DirectX/Deviceclass.h"
#include"../DirectX/CommandQueueclass.h"
#include"../DirectX/Swapchainclassh.h"
#include"../DirectX/DescripterHeapclass.h"
#include"../DirectX/ComandListclass.h"
#include"../DirectX/ComandAllocaterclass.h"
#include"../DirectX/ReaderTargetclass.h"
#include"../DirectX/Fenceclass.h"
#include <cassert>

namespace {
	constexpr std::string_view App_name = "App_window";
}
class App
{
public:
	App() = default;
	~App() = default;

	[[nodiscard]] bool initialize(HINSTANCE instance)noexcept {
		//
		if (S_OK != window_.Create(instance, 1280, 720, App_name)) {
			assert(false && "ウィンドウの生成に失敗しました");
			return false;
		}

		//
		if (!dxgi_.SetDisplayAdapter()) {
			assert(false && "DXGIのアダプター設定の失敗しました");
			return false;
		}

		if (!device_.Create(dxgi_)) {
			assert(false && "Deviceの設定に失敗しました");
			return false;
		}

		if (!command_.create(device_)) {
			assert(false && "コマンドキューの設定に失敗しました");
			return false;
		}

		if (!Swap_.create(dxgi_, command_, window_)) {
			assert(false && "スワップチェーンの設定に失敗しました");
			return false;
		}

		if (!descriptor_.create(device_, D3D12_DESCRIPTOR_HEAP_TYPE_RTV, Swap_.getDesc().BufferCount)) {
			assert(false && "ディスクリプターヒープの設定に失敗しました");
			return false;
		}

		if (!rendertarget_.create(device_, Swap_, descriptor_)) {
			assert(false && "レンダーターゲットの設定に失敗");
			return false;
		}

		if (!commandAllocator_[0].create(device_, D3D12_COMMAND_LIST_TYPE_DIRECT)) {
			assert(false && "コマンドアロケーターの設定に失敗");
			return false;
		}

		if (!commandAllocator_[1].create(device_, D3D12_COMMAND_LIST_TYPE_DIRECT)) {
			assert(false && "コマンドアロケーターの設定に失敗");
			return false;
		}

		if (!commandList_.create(device_, commandAllocator_[0])) {
			assert(false && "コマンドリストの設定に失敗");
			return false;
		}

		if (!fence_.create(device_)) {
			assert(false && "フェンスの設定に失敗");
			return false;
		}

		return true;
	}

	void Loop()noexcept {
		while (window_.MessageLoop()) {
			
			const auto BackBufferIndex = Swap_.get()->GetCurrentBackBufferIndex();

			if (frameFenceValue_[BackBufferIndex] != 0) {
				fence_.wait(frameFenceValue_[BackBufferIndex]);
			}

			commandAllocator_[BackBufferIndex].reset();
			commandList_.reset(commandAllocator_[BackBufferIndex]);

			auto pToRT = resourceBarrier(rendertarget_.get(BackBufferIndex), D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);
			commandList_.get()->ResourceBarrier(1, &pToRT);

			D3D12_CPU_DESCRIPTOR_HANDLE handles[] = { rendertarget_.getDescripterHandle(device_,descriptor_,BackBufferIndex) };
			commandList_.get()->OMSetRenderTargets(1, handles, false, nullptr);

			const float clearColer[] = { 1.0f, 1.0f, 0.0f, 1.0f };
			commandList_.get()->ClearRenderTargetView(handles[0], clearColer, 0, nullptr);

			auto rtRToP = resourceBarrier(rendertarget_.get(BackBufferIndex), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
			commandList_.get()->ResourceBarrier(1, &rtRToP);

			commandList_.get()->Close();

			ID3D12CommandList* ppCommandList[] = { commandList_.get() };
			command_.get()->ExecuteCommandLists(_countof(ppCommandList), ppCommandList);

			Swap_.get()->Present(1, 0);

			command_.get()->Signal(fence_.get(), nextFenceValue_);
			frameFenceValue_[BackBufferIndex] = nextFenceValue_;
			nextFenceValue_++;
		}
	}

	//----------------------------------------------------------------------------------------
	D3D12_RESOURCE_BARRIER resourceBarrier(ID3D12Resource* resource, D3D12_RESOURCE_STATES from, D3D12_RESOURCE_STATES to)noexcept {
		D3D12_RESOURCE_BARRIER barrier{};
		barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
		barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
		barrier.Transition.pResource = resource;
		barrier.Transition.StateBefore = from;
		barrier.Transition.StateAfter = to;
		barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;

		return barrier;
	}
private:
	window window_{};
	DXGI dxgi_{};
	Device device_{};
	CommandQueue command_{};
	Swapchain Swap_{};
	DescripterHeap descriptor_{};
	RenderTarget rendertarget_{};
	ComandAllocater commandAllocator_[2]{};
	CommandList commandList_{};

	Fence fence_{};
	UINT64 frameFenceValue_[2]{};
	UINT64 nextFenceValue_ = 1;
};

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	App app;
	if (!app.initialize(hInstance)) {
		assert(false && "ウィンドウの初期化に失敗しました");
	}

	app.Loop();
	return 0;
}