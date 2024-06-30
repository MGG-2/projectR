#pragma once

#include <d3d12.h>
#include <dxgi1_4.h>
#include <string>

bool LoadTextureFromFile(const char* filename, ID3D12Device* d3d_device, D3D12_CPU_DESCRIPTOR_HANDLE srv_cpu_handle, ID3D12Resource** out_tex_resource, int* out_width, int* out_height);
bool LoadBackgroundImage(const std::string& filename, ID3D12Device* device, ID3D12GraphicsCommandList* commandList, ID3D12CommandQueue* commandQueue);
void RenderBackground(ID3D12GraphicsCommandList* commandList);
