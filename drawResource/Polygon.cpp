#include"Polygonclass.h"
#include<cassert>
#include<DirectXMath.h>

namespace {
    struct Vertex {
        DirectX::XMFLOAT3 position;
        DirectX::XMFLOAT4 color; 
    };

    Vertex T[] = {{ {-0.5f, -0.5f, 0} , {  0,  0, 1.0, 1.0} },{ { 0.5f,  0.5f,  0} , {  0,  0, 1.0, 1.0} },{ { 0.5f, -0.5f,  0} , {  0,  0, 1.0, 1.0} },};
    Vertex P[] = { { {-0.5f, -0.5f, 0} , {  1,  0,   0, 1.0} },{ { -0.5f,  0.5f,  0} , {  1,  0,   0, 1.0} },{ { 0.5f, 0.5f,  0} , {  1,  0,   0, 1.0} }, };
}

MakePolygon :: ~MakePolygon()
{
    if (vertexBuffer_) {
        vertexBuffer_->Release();
        vertexBuffer_ = nullptr;
    }
    if (indexBuffer_) {
        indexBuffer_->Release();
        indexBuffer_ = nullptr;
    }
}

[[nodiscard]] bool MakePolygon::create(const Device& device)noexcept
{
    for (int i= 0; i< 2; i++)
    {
        if (!createVertexBuffer(device, i)) {
            return false;
        }
        if (!createIndexBuffer(device, i)) {
            return false;
        }
    }
    return true;
}

[[nodiscard]] void MakePolygon::draw(const CommandList& commandList)noexcept
{
    commandList.get()->IASetVertexBuffers(0, 1, &vertexBufferView_[0]);
    commandList.get()->IASetIndexBuffer(&indexBufferView_[0]);
    commandList.get()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    commandList.get()->DrawIndexedInstanced(3, 1, 0, 0, 0);

    commandList.get()->IASetVertexBuffers(0, 1, &vertexBufferView_[1]);
    commandList.get()->IASetIndexBuffer(&indexBufferView_[1]);
    commandList.get()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    commandList.get()->DrawIndexedInstanced(3, 1, 0, 0, 0);
}

[[nodiscard]] bool MakePolygon::createVertexBuffer(const Device& device, const int num)noexcept
{
    auto vertexBuffersize = sizeof(T);
    
    D3D12_HEAP_PROPERTIES heapPropaty{};
    heapPropaty.Type = D3D12_HEAP_TYPE_UPLOAD;
    heapPropaty.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
    heapPropaty.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
    heapPropaty.CreationNodeMask = 1;
    heapPropaty.VisibleNodeMask = 1;

    D3D12_RESOURCE_DESC resourceDesc{};
    resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
    resourceDesc.Alignment = 0;
    resourceDesc.Width = vertexBuffersize;
    resourceDesc.Height = 1;
    resourceDesc.DepthOrArraySize = 1;
    resourceDesc.MipLevels = 1;
    resourceDesc.Format = DXGI_FORMAT_UNKNOWN;
    resourceDesc.SampleDesc.Count = 1;
    resourceDesc.SampleDesc.Quality = 0;
    resourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
    resourceDesc.Flags = D3D12_RESOURCE_FLAG_NONE;

    HRESULT hr = device.get()->CreateCommittedResource(
        &heapPropaty, D3D12_HEAP_FLAG_NONE, &resourceDesc, D3D12_RESOURCE_STATE_GENERIC_READ, 
        nullptr, IID_PPV_ARGS(&vertexBuffer_)
    );
    if (FAILED(hr)) {
        assert(false && "頂点バッファの作成に失敗");
        return false;
    }

    Vertex* data{};
    hr = vertexBuffer_->Map(0, nullptr, reinterpret_cast<void**>(&data));
    if (FAILED(hr)) {
        assert(false && "頂点バッファのマップに失敗");
        return false;
    }

    
    if (num != 0) {
        memcpy_s(data, vertexBuffersize, P, vertexBuffersize);
    }
    else {
        memcpy_s(data, vertexBuffersize, T, vertexBuffersize);
    }

    vertexBuffer_->Unmap(0, nullptr);

    vertexBufferView_[num].BufferLocation = vertexBuffer_->GetGPUVirtualAddress();
    vertexBufferView_[num].SizeInBytes = vertexBuffersize;
    vertexBufferView_[num].StrideInBytes = sizeof(Vertex);

    return true;
}

[[nodiscard]] bool MakePolygon::createIndexBuffer(const Device& device,const int num)noexcept
{
    uint16_t triangle[] = { 0, 1, 2};

    const auto indexBuffersize = sizeof(triangle);


    D3D12_HEAP_PROPERTIES heapProperty{};
    heapProperty.Type = D3D12_HEAP_TYPE_UPLOAD;
    heapProperty.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
    heapProperty.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
    heapProperty.CreationNodeMask = 1;
    heapProperty.VisibleNodeMask = 1;

    // リソースの設定を行う
    D3D12_RESOURCE_DESC resourceDesc{};
    resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
    resourceDesc.Alignment = 0;
    resourceDesc.Width = indexBuffersize;
    resourceDesc.Height = 1;
    resourceDesc.DepthOrArraySize = 1;
    resourceDesc.MipLevels = 1;
    resourceDesc.Format = DXGI_FORMAT_UNKNOWN;
    resourceDesc.SampleDesc.Count = 1;
    resourceDesc.SampleDesc.Quality = 0;
    resourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
    resourceDesc.Flags = D3D12_RESOURCE_FLAG_NONE;

    HRESULT hr = device.get()->CreateCommittedResource(
        &heapProperty, D3D12_HEAP_FLAG_NONE, &resourceDesc, D3D12_RESOURCE_STATE_GENERIC_READ,
        nullptr, IID_PPV_ARGS(&indexBuffer_)
    );
    if (FAILED(hr)) {
        assert(false && "インデックバッファの作成に失敗");
        return false;
    }

    uint16_t* data{};
    hr = indexBuffer_->Map(0, nullptr, reinterpret_cast<void**>(&data));
    if (FAILED(hr)) {
        assert(false && "頂点バッファのマップに失敗");
        return false;
    }

    memcpy_s(data, indexBuffersize, triangle, indexBuffersize);

    indexBuffer_->Unmap(0, nullptr);

    indexBufferView_[num].BufferLocation = indexBuffer_->GetGPUVirtualAddress();
    indexBufferView_[num].SizeInBytes = indexBuffersize;
    indexBufferView_[num].Format = DXGI_FORMAT_R16_UINT;

    return true;
}