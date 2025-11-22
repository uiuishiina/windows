#include"Useshaderclass.h"
#include<cassert>
#include<string>
#include<D3Dcompiler.h>
#pragma comment(lib, "d3dcompiler.lib")


Shader :: ~Shader()
{
	if (vertexShader_) {
		vertexShader_->Release();
		vertexShader_ = nullptr;
	}
	if (pixelShader_) {
		pixelShader_->Release();
		pixelShader_ = nullptr;
	}
}

//シェーダー作成関数
[[nodiscard]] bool Shader::create(const Device& device)noexcept
{
	const std::string filePath = "../shader/shader.hlsl";
	const std::wstring temp = std::wstring(filePath.begin(), filePath.end());

	ID3DBlob* error_{};

	auto Compile = D3DCompileFromFile(temp.data(), nullptr, nullptr, "vs", "vs_5_0", D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, 0, &vertexShader_, &error_);
	if (FAILED(Compile)) {
		char* p = static_cast<char*>(error_->GetBufferPointer());
		assert(false && "頂点シェーダーのコンパイルに失敗");
	}
	Compile = D3DCompileFromFile(temp.data(), nullptr, nullptr, "ps", "ps_5_0", D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, 0, &pixelShader_, &error_);
	if (FAILED(Compile)) {
		char* p = static_cast<char*>(error_->GetBufferPointer());
		assert(false && "ピクセルシェーダーのコンパイルに失敗");
	}

	if (error_) {
		error_->Release();
	}

	return true;
}

//頂点シェーダーを返す
[[nodiscard]] ID3DBlob* Shader::getVertexShader()const noexcept
{
	if (!vertexShader_) {
		assert(false && "頂点シェーダーが未作成");
		return nullptr;
	}
	return vertexShader_;
}

//ピクセルシェーダーを返す
[[nodiscard]] ID3DBlob* Shader::getPixelShader()const noexcept
{
	if (!pixelShader_) {
		assert(false && "ピクセルシェーダーが未作成");
		return nullptr;
	}
	return pixelShader_;
}