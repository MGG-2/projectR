#include "background.h"
#include "../../imgui/imgui.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <iostream>

static ID3D12Resource* g_backgroundTexture = nullptr;
static D3D12_CPU_DESCRIPTOR_HANDLE g_backgroundSrvCpuDescHandle;
static D3D12_GPU_DESCRIPTOR_HANDLE g_backgroundSrvGpuDescHandle;
extern ID3D12Device* g_pd3dDevice;
extern ID3D12DescriptorHeap* g_pd3dSrvDescHeap;

inline UINT64 GetRequiredIntermediateSize(ID3D12Resource* pResource, UINT FirstSubresource, UINT NumSubresources) {
    D3D12_RESOURCE_DESC desc = pResource->GetDesc();
    UINT64 RequiredSize = 0;
    ID3D12Device* pDevice = nullptr;
    pResource->GetDevice(IID_PPV_ARGS(&pDevice));
    pDevice->GetCopyableFootprints(&desc, FirstSubresource, NumSubresources, 0, nullptr, nullptr, nullptr, &RequiredSize);
    return RequiredSize;
}

void UpdateSubresources(
    ID3D12GraphicsCommandList* pCmdList,
    ID3D12Resource* pDstResource,
    ID3D12Resource* pIntermediate,
    UINT64 IntermediateOffset,
    UINT FirstSubresource,
    UINT NumSubresources,
    D3D12_SUBRESOURCE_DATA* pSrcData)
{
    UINT64 RequiredSize = GetRequiredIntermediateSize(pDstResource, FirstSubresource, NumSubresources);
    BYTE* pData;
    pIntermediate->Map(0, nullptr, reinterpret_cast<void**>(&pData));
    for (UINT i = 0; i < NumSubresources; ++i) {
        D3D12_MEMCPY_DEST DestData = { pData + pSrcData[i].SlicePitch * i, pSrcData[i].RowPitch, pSrcData[i].SlicePitch };
        memcpy(DestData.pData, pSrcData[i].pData, pSrcData[i].SlicePitch);
    }
    pIntermediate->Unmap(0, nullptr);

    D3D12_RESOURCE_BARRIER barrier = {};
    barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
    barrier.Transition.pResource = pDstResource;
    barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_COPY_DEST;
    barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;
    barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
    pCmdList->ResourceBarrier(1, &barrier);
}

bool LoadTextureFromFile(const char* filename, ID3D12Device* d3d_device, D3D12_CPU_DESCRIPTOR_HANDLE srv_cpu_handle, ID3D12Resource** out_tex_resource, int* out_width, int* out_height)
{
    int image_width = 0;
    int image_height = 0;
    unsigned char* image_data = stbi_load(filename, &image_width, &image_height, NULL, 4);
    if (image_data == NULL) {
        std::cerr << "Failed to load image: " << filename << std::endl;
        return false;
    }

    D3D12_HEAP_PROPERTIES props = {};
    props.Type = D3D12_HEAP_TYPE_DEFAULT;
    props.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
    props.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;

    D3D12_RESOURCE_DESC desc = {};
    desc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
    desc.Width = image_width;
    desc.Height = image_height;
    desc.DepthOrArraySize = 1;
    desc.MipLevels = 1;
    desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    desc.SampleDesc.Count = 1;
    desc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
    desc.Flags = D3D12_RESOURCE_FLAG_NONE;

    HRESULT hr = d3d_device->CreateCommittedResource(&props, D3D12_HEAP_FLAG_NONE, &desc, D3D12_RESOURCE_STATE_COPY_DEST, NULL, IID_PPV_ARGS(out_tex_resource));
    if (FAILED(hr)) {
        std::cerr << "Failed to create committed resource: " << hr << std::endl;
        stbi_image_free(image_data);
        return false;
    }

    UINT uploadPitch = (image_width * 4 + D3D12_TEXTURE_DATA_PITCH_ALIGNMENT - 1u) & ~(D3D12_TEXTURE_DATA_PITCH_ALIGNMENT - 1u);
    UINT uploadSize = image_height * uploadPitch;
    desc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
    desc.Width = uploadSize;
    desc.Height = 1;
    desc.DepthOrArraySize = 1;
    desc.MipLevels = 1;
    desc.Format = DXGI_FORMAT_UNKNOWN;
    desc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

    props.Type = D3D12_HEAP_TYPE_UPLOAD;

    ID3D12Resource* uploadBuffer = NULL;
    hr = d3d_device->CreateCommittedResource(&props, D3D12_HEAP_FLAG_NONE, &desc, D3D12_RESOURCE_STATE_GENERIC_READ, NULL, IID_PPV_ARGS(&uploadBuffer));
    if (FAILED(hr)) {
        std::cerr << "Failed to create upload buffer: " << hr << std::endl;
        stbi_image_free(image_data);
        return false;
    }

    void* mapped = NULL;
    D3D12_RANGE range = { 0, uploadSize };
    hr = uploadBuffer->Map(0, &range, &mapped);
    if (FAILED(hr)) {
        std::cerr << "Failed to map upload buffer: " << hr << std::endl;
        uploadBuffer->Release();
        stbi_image_free(image_data);
        return false;
    }

    for (int y = 0; y < image_height; y++)
        memcpy((void*)((uintptr_t)mapped + y * uploadPitch), image_data + y * image_width * 4, image_width * 4);
    uploadBuffer->Unmap(0, &range);

    D3D12_TEXTURE_COPY_LOCATION srcLocation = {};
    srcLocation.pResource = uploadBuffer;
    srcLocation.Type = D3D12_TEXTURE_COPY_TYPE_PLACED_FOOTPRINT;
    srcLocation.PlacedFootprint.Footprint.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    srcLocation.PlacedFootprint.Footprint.Width = image_width;
    srcLocation.PlacedFootprint.Footprint.Height = image_height;
    srcLocation.PlacedFootprint.Footprint.RowPitch = uploadPitch;

    D3D12_TEXTURE_COPY_LOCATION dstLocation = {};
    dstLocation.pResource = *out_tex_resource;
    dstLocation.Type = D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX;
    dstLocation.SubresourceIndex = 0;

    D3D12_RESOURCE_BARRIER barrier = {};
    barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
    barrier.Transition.pResource = *out_tex_resource;
    barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_COPY_DEST;
    barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;

    ID3D12Fence* fence = NULL;
    hr = d3d_device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence));
    if (FAILED(hr)) {
        std::cerr << "Failed to create fence: " << hr << std::endl;
        uploadBuffer->Release();
        stbi_image_free(image_data);
        return false;
    }

    HANDLE event = CreateEvent(0, 0, 0, 0);
    if (event == NULL) {
        std::cerr << "Failed to create event." << std::endl;
        fence->Release();
        uploadBuffer->Release();
        stbi_image_free(image_data);
        return false;
    }

    D3D12_COMMAND_QUEUE_DESC queueDesc = {};
    queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;

    ID3D12CommandQueue* cmdQueue = NULL;
    hr = d3d_device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&cmdQueue));
    if (FAILED(hr)) {
        std::cerr << "Failed to create command queue: " << hr << std::endl;
        CloseHandle(event);
        fence->Release();
        uploadBuffer->Release();
        stbi_image_free(image_data);
        return false;
    }

    ID3D12CommandAllocator* cmdAlloc = NULL;
    hr = d3d_device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&cmdAlloc));
    if (FAILED(hr)) {
        std::cerr << "Failed to create command allocator: " << hr << std::endl;
        cmdQueue->Release();
        CloseHandle(event);
        fence->Release();
        uploadBuffer->Release();
        stbi_image_free(image_data);
        return false;
    }

    ID3D12GraphicsCommandList* cmdList = NULL;
    hr = d3d_device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, cmdAlloc, NULL, IID_PPV_ARGS(&cmdList));
    if (FAILED(hr)) {
        std::cerr << "Failed to create command list: " << hr << std::endl;
        cmdAlloc->Release();
        cmdQueue->Release();
        CloseHandle(event);
        fence->Release();
        uploadBuffer->Release();
        stbi_image_free(image_data);
        return false;
    }

    cmdList->CopyTextureRegion(&dstLocation, 0, 0, 0, &srcLocation, NULL);
    cmdList->ResourceBarrier(1, &barrier);

    hr = cmdList->Close();
    if (FAILED(hr)) {
        std::cerr << "Failed to close command list: " << hr << std::endl;
        cmdList->Release();
        cmdAlloc->Release();
        cmdQueue->Release();
        CloseHandle(event);
        fence->Release();
        uploadBuffer->Release();
        stbi_image_free(image_data);
        return false;
    }

    cmdQueue->ExecuteCommandLists(1, (ID3D12CommandList* const*)&cmdList);
    hr = cmdQueue->Signal(fence, 1);
    if (FAILED(hr)) {
        std::cerr << "Failed to signal fence: " << hr << std::endl;
        cmdList->Release();
        cmdAlloc->Release();
        cmdQueue->Release();
        CloseHandle(event);
        fence->Release();
        uploadBuffer->Release();
        stbi_image_free(image_data);
        return false;
    }

    fence->SetEventOnCompletion(1, event);
    WaitForSingleObject(event, INFINITE);

    cmdList->Release();
    cmdAlloc->Release();
    cmdQueue->Release();
    CloseHandle(event);
    fence->Release();
    uploadBuffer->Release();

    D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
    srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
    srvDesc.Texture2D.MipLevels = 1;
    srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
    d3d_device->CreateShaderResourceView(*out_tex_resource, &srvDesc, srv_cpu_handle);

    *out_width = image_width;
    *out_height = image_height;
    stbi_image_free(image_data);

    return true;
}