#pragma once
#include"Deviceclass.h"
#include"Useshaderclass.h"
#include"Root_signatureclass.h"

class PiPline
{
public:
	PiPline() = default;

	//
	~PiPline();

	//
	[[nodiscard]] bool create(const Device& device,const Shader& shader,const Root& root)noexcept;

	//
	[[nodiscard]] ID3D12PipelineState* get()const noexcept;
private:
	ID3D12PipelineState* piplinestate_{};
};