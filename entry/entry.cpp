#include"../window/windowclass.h"
#include"../DirectX/DXGIclass.h"
#include"../DirectX/Deviceclass.h"
#include"../DirectX/CommandQueueclass.h"
#include"../DirectX/Swapchainclassh.h"
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
	}

	void Loop()noexcept {
		while (window_.MessageLoop()) {
			//Swap_.get()->Present(1, 0);
		}
	}
private:
	window window_{};
	DXGI dxgi_{};
	Device device_{};
	CommandQueue command_{};
	Swapchain Swap_{};
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