#pragma once
#include"Deviceclass.h"

class Root 
{
public:
	Root() = default;

	//
	~Root();

	//
	[[nodiscrad]] bool create(const Device& device)noexcept;

	//
	[[nodiscard]] ID3D12RootSignature* get()const noexcept;
private:
	ID3D12RootSignature* rootSignature_{};
};