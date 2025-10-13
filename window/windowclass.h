#pragma once
#include<Windows.h>
#include<string>

class window
{
public:
	window() = default;
	~window() = default;

	//-----------------------------------------------------------------
	/*
	*@brief ウィンドウ生成関数
	*@param instancse インスタンス
	*@param width 横幅
	*@param heigth 高さ
	*@param name ウィンドウの名前
	*@return 成功可否をintみたいなので返す
	*/
	[[nodiscard]] HRESULT Create(HINSTANCE instance, int width, int heigth, std::string_view name)noexcept;
	//-----------------------------------------------------------------
	/*
	*@brief メッセージループ関数
	*/
	[[nodiscard]] bool MessageLoop()const noexcept;
private:
	//-----------------------------------------------------------------
	/*
	*@brief ウィンドウハンドルクラス
	* 作るウィンドウのクラスみたいなのを宣言しとく
	*/
	HWND hwnd_{};
};
