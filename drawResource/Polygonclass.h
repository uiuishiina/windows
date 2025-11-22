#pragma once

#include"../DirectX/Deviceclass.h"
#include"../DirectX/ComandListclass.h"

class MakePolygon
{
public:
	MakePolygon() = default;

	//デストラクタ
	~MakePolygon();

	//ポリゴン作成
	[[nodiscard]] bool create(const Device& device)noexcept;

	//ポリゴン描画
	[[nodiscard]] void draw(const CommandList& commandList)noexcept;

	//頂点バッファ
	[[nodiscard]] bool createVertexBuffer(const Device& device)noexcept;

	//インデックスバッファ
	[[nodiscard]] bool createIndexBuffer(const Device& device)noexcept;
private:
	ID3D12Resource* vertexBuffer_{};
	ID3D12Resource* indexBuffer_{};

	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_ = {};
	D3D12_INDEX_BUFFER_VIEW indexBufferView_ = {};
};
