#include"Root_signatureclass.h"
#include<cassert>

Root::~Root()
{
	if (rootSignature_) {
		rootSignature_->Release();
		rootSignature_ = nullptr;
	}
}


[[nodiscrad]] bool Root::create(const Device& device)noexcept
{
	D3D12_ROOT_SIGNATURE_DESC rootDesc{};
	rootDesc.NumParameters = 0;
	rootDesc.pParameters = nullptr;
	rootDesc.NumStaticSamplers = 0;
	rootDesc.pStaticSamplers = nullptr;
	rootDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

	ID3DBlob* signature{};
	HRESULT hr = D3D12SerializeRootSignature(
		&rootDesc, D3D_ROOT_SIGNATURE_VERSION_1,
		&signature, nullptr
	);

	bool success = SUCCEEDED(hr);
	if (!success) {
		assert(false && "ルートシグネチャーのシリアライズ化に失敗");
	}
	else {
		hr = device.get()->CreateRootSignature(
			0, signature->GetBufferPointer(),
			signature->GetBufferSize(),
			IID_PPV_ARGS(&rootSignature_)
		);
	}
	success &= SUCCEEDED(hr);
	if (!success) {
		assert(false && "ルートシグネチャーの生成に失敗");
	}

	if (signature) {
		signature->Release();
	}

	return success;
}

[[nodiscard]] ID3D12RootSignature* Root::get()const noexcept
{
	if (!rootSignature_) {
		assert(false && "ルートシグネチャーが生成されていません");
		return nullptr;
	}
	return rootSignature_;
}