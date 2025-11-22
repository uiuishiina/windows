#include"Pipline_stateclass.h"
#include<cassert>

PiPline::~PiPline(){
	if (piplinestate_) {
		piplinestate_->Release();
		piplinestate_ = nullptr;
	}
}


[[nodiscard]] bool PiPline::create(const Device& device, const Shader& shader, const Root& root)noexcept
{
	D3D12_INPUT_ELEMENT_DESC inputDesc[] = {
		{"POSITION",	0,	DXGI_FORMAT_R32G32B32_FLOAT,	0,	0,	D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,	0},
		{	"COLOR",	0,	DXGI_FORMAT_R32G32B32A32_FLOAT,	0, 12,	D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,	0}
	};

	D3D12_RASTERIZER_DESC rasterizerDesc{};
	rasterizerDesc.FillMode = D3D12_FILL_MODE_SOLID;
	rasterizerDesc.CullMode = D3D12_CULL_MODE_BACK;
	rasterizerDesc.FrontCounterClockwise = false;
	rasterizerDesc.DepthBias = D3D12_DEFAULT_DEPTH_BIAS;
	rasterizerDesc.DepthBiasClamp = D3D12_DEFAULT_DEPTH_BIAS_CLAMP;
	rasterizerDesc.SlopeScaledDepthBias = D3D12_DEFAULT_SLOPE_SCALED_DEPTH_BIAS;
	rasterizerDesc.DepthClipEnable = true;
	rasterizerDesc.MultisampleEnable = false;
	rasterizerDesc.AntialiasedLineEnable = false;
	rasterizerDesc.ForcedSampleCount = 0;
	rasterizerDesc.ConservativeRaster = D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;

	const D3D12_RENDER_TARGET_BLEND_DESC defaultRenderTargetDesc = {
	FALSE,FALSE,
	D3D12_BLEND_ONE,D3D12_BLEND_ZERO,D3D12_BLEND_OP_ADD,
	D3D12_BLEND_ONE,D3D12_BLEND_ZERO,D3D12_BLEND_OP_ADD,
	D3D12_LOGIC_OP_NOOP,D3D12_COLOR_WRITE_ENABLE_ALL
	};
	D3D12_BLEND_DESC blendDesc{};
	blendDesc.AlphaToCoverageEnable = false;
	blendDesc.IndependentBlendEnable = false;
	for (UINT i = 0; i < D3D12_SIMULTANEOUS_RENDER_TARGET_COUNT; i++)
	{
		blendDesc.RenderTarget[i] = defaultRenderTargetDesc;
	}

	D3D12_GRAPHICS_PIPELINE_STATE_DESC pstateDesc{};
	pstateDesc.InputLayout = { inputDesc,_countof(inputDesc) };
	pstateDesc.pRootSignature = root.get();
	pstateDesc.VS = { shader.getVertexShader()->GetBufferPointer(),shader.getVertexShader()->GetBufferSize() };
	pstateDesc.PS = { shader.getPixelShader()->GetBufferPointer(),shader.getPixelShader()->GetBufferSize() };
	pstateDesc.RasterizerState = rasterizerDesc;
	pstateDesc.BlendState = blendDesc;
	pstateDesc.DepthStencilState.DepthEnable = false;
	pstateDesc.DepthStencilState.StencilEnable = false;
	pstateDesc.SampleMask = UINT_MAX;
	pstateDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	pstateDesc.NumRenderTargets = 1;
	pstateDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
	pstateDesc.SampleDesc.Count = 1;
	HRESULT hr = device.get()->CreateGraphicsPipelineState(&pstateDesc, IID_PPV_ARGS(&piplinestate_));
	if (FAILED(hr)) {
		assert(false && "パイプラインステートの作成に失敗");
		return false;
	}
	return true;

}

[[nodiscard]] ID3D12PipelineState* PiPline ::get()const noexcept
{
	if (!piplinestate_) {
		assert(false && "パイプラインステートが未作成");
	}
	return piplinestate_;
}
