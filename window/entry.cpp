#include"windowclass.h"
#include <cassert>

namespace {
	constexpr std::string_view App_name = "App_window";
}
class App
{
public:
	App() = default;
	~App() = default;

	[[nodiscard]] HRESULT initialize(HINSTANCE instance)noexcept{
		return window_.Create(instance, 1280, 720, App_name);
	}

	void Loop()noexcept {
		while (window_.MessageLoop()) {

		}
	}
private:
	window window_;
};

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	App app;
	if (app.initialize(hInstance)) {
		assert(false && "ウィンドウの初期化に失敗しました");
	}

	app.Loop();
	return 0;
}