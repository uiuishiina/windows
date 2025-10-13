#include"windowclass.h"

//-------------------------------------------------------------------
/*コールバック関数
*@brief ウィンドウプロシージャ
*@param hwnd ウィンドウハンドル
*@param msg メッセージ
*@param wparam メッセージパラメーター
*@param lparam メッセージパラメーター
*@return 結果
*/
LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	switch (msg)//メッセージによって分岐
	{
	case WM_DESTROY://ウィンドウ終了時
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hwnd, msg, wparam, lparam);//switchで拾わないやつを処理
}

//-------------------------------------------------------------------
/*ウィンドウ作成関数・・・windowclassに宣言済み
**@brief ウィンドウ生成
*@param instancse インスタンス
*@param width 横幅
*@param heigth 高さ
*@param name ウィンドウの名前
*@return 成功可否をintみたいなので返す
*/
[[nodiscard]] HRESULT window::Create(HINSTANCE instance, int width, int heigth, std::string_view name)noexcept{
	//ウィンドウの定義
	WNDCLASS wc{};//クラス作成
	wc.lpfnWndProc = WindowProc;
	wc.hInstance = instance;
	wc.lpszClassName = name.data();//安全なstringにしてる
	wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);//ウィンドウの色変更

	RegisterClass(&wc);//クラス内に設定

	//ウィンドウ作成
	hwnd_ = CreateWindow(wc.lpszClassName, wc.lpszClassName,
		WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, width, heigth,
		NULL, NULL, instance, NULL);

	if (!hwnd_){
		return E_FAIL;//できてないならfalse
	}

	ShowWindow(hwnd_, SW_SHOW);//ウィンドウ表示

	UpdateWindow(hwnd_);//ウィンドウ更新

	return S_OK;//できてるのでtrue
}

//-------------------------------------------------------------------
[[nodiscard]] bool window::MessageLoop()const noexcept{
	MSG msg{};
	while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
		if(msg.message == WM_QUIT) {
			return false;
		}

		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return true;
}