#pragma once
#include"Deviceclass.h"

class Shader
{
public:
	Shader() = default;

	//デストラクタ
	~Shader();

	//シェーダーを作成
	[[nodiscard]] bool create(const Device& device)noexcept;

	//頂点シェーダーの取得
	[[nodiscard]] ID3DBlob* getVertexShader()const noexcept;

	//ピクセルシェーダーの取得
	[[nodiscard]] ID3DBlob* getPixelShader()const noexcept;
private:
	ID3DBlob* vertexShader_{};//頂点シェーダーのアドレス
	ID3DBlob* pixelShader_{};//ピクセルシェーダーのアドレス
};