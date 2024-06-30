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

bool LoadBackgroundImage(const std::string& filename, ID3D12Device* device, ID3D12GraphicsCommandList* commandList, ID3D12CommandQueue* commandQueue) {
    std::string absoluteFilename = filename;
    std::cout << "Loading background image from " << absoluteFilename << std::endl;

    int width = 0, height = 0, channels = 0;
    unsigned char* imageData = stbi_load(absoluteFilename.c_str(), &width, &height, &channels, STBI_rgb_alpha);
    if (!imageData) {
        std::cerr << "Failed to load image: " << absoluteFilename << std::endl;
        std::cerr << "stb_image failure reason: " << stbi_failure_reason() << std::endl;
        return false;
    }

    D3D12_RESOURCE_DESC textureDesc = {};
    textureDesc.MipLevels = 1;
    textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    textureDesc.Width = width;
    textureDesc.Height = height;
    textureDesc.Flags = D3D12_RESOURCE_FLAG_NONE;
    textureDesc.DepthOrArraySize = 1;
    textureDesc.SampleDesc.Count = 1;
    textureDesc.SampleDesc.Quality = 0;
    textureDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;

    D3D12_HEAP_PROPERTIES heapProps = {};
    heapProps.Type = D3D12_HEAP_TYPE_DEFAULT;

    if (device->CreateCommittedResource(
        &heapProps,
        D3D12_HEAP_FLAG_NONE,
        &textureDesc,
        D3D12_RESOURCE_STATE_COPY_DEST,
        nullptr,
        IID_PPV_ARGS(&g_backgroundTexture)) != S_OK) {
        std::cerr << "Failed to create committed resource for texture." << std::endl;
        stbi_image_free(imageData);
        return false;
    }

    UINT64 uploadBufferSize = GetRequiredIntermediateSize(g_backgroundTexture, 0, 1);

    D3D12_HEAP_PROPERTIES uploadHeapProps = {};
    uploadHeapProps.Type = D3D12_HEAP_TYPE_UPLOAD;

    D3D12_RESOURCE_DESC bufferDesc = {};
    bufferDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
    bufferDesc.Width = uploadBufferSize;
    bufferDesc.Height = 1;
    bufferDesc.DepthOrArraySize = 1;
    bufferDesc.MipLevels = 1;
    bufferDesc.Format = DXGI_FORMAT_UNKNOWN;
    bufferDesc.SampleDesc.Count = 1;
    bufferDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

    ID3D12Resource* uploadBuffer = nullptr;
    if (device->CreateCommittedResource(
        &uploadHeapProps,
        D3D12_HEAP_FLAG_NONE,
        &bufferDesc,
        D3D12_RESOURCE_STATE_GENERIC_READ,
        nullptr,
        IID_PPV_ARGS(&uploadBuffer)) != S_OK) {
        std::cerr << "Failed to create upload buffer." << std::endl;
        stbi_image_free(imageData);
        return false;
    }

    D3D12_SUBRESOURCE_DATA textureData = {};
    textureData.pData = imageData;
    textureData.RowPitch = width * 4;
    textureData.SlicePitch = textureData.RowPitch * height;

    UpdateSubresources(commandList, g_backgroundTexture, uploadBuffer, 0, 0, 1, &textureData);

    D3D12_RESOURCE_BARRIER barrier = {};
    barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
    barrier.Transition.pResource = g_backgroundTexture;
    barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_COPY_DEST;
    barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;
    barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
    commandList->ResourceBarrier(1, &barrier);

    stbi_image_free(imageData);

    D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc = {};
    srvHeapDesc.NumDescriptors = 1;
    srvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
    srvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
    if (device->CreateDescriptorHeap(&srvHeapDesc, IID_PPV_ARGS(&g_pd3dSrvDescHeap)) != S_OK) {
        std::cerr << "Failed to create descriptor heap." << std::endl;
        return false;
    }

    g_backgroundSrvCpuDescHandle = g_pd3dSrvDescHeap->GetCPUDescriptorHandleForHeapStart();
    g_backgroundSrvGpuDescHandle = g_pd3dSrvDescHeap->GetGPUDescriptorHandleForHeapStart();

    D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
    srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
    srvDesc.Format = textureDesc.Format;
    srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
    srvDesc.Texture2D.MipLevels = 1;
    device->CreateShaderResourceView(g_backgroundTexture, &srvDesc, g_backgroundSrvCpuDescHandle);

    std::cout << "Background image loaded successfully." << std::endl;
    return true;
}

void RenderBackground(ID3D12GraphicsCommandList* commandList) {
    ImGui::GetBackgroundDrawList()->AddImage(
        (void*)g_backgroundSrvGpuDescHandle.ptr,
        ImVec2(0, 0),
        ImVec2(ImGui::GetIO().DisplaySize.x, ImGui::GetIO().DisplaySize.y),
        ImVec2(0, 0),
        ImVec2(1, 1)
    );
}
