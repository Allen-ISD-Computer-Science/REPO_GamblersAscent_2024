#pragma once
#include <d3d11.h>


class Texture {
public:
    Texture(const char* filename, ID3D11ShaderResourceView** out_srv, int* out_width, int* out_height,
        ID3D11Device* g_pd3dDevice);
	ID3D11Device* g_pd3dDevice;
    bool renderSuccess;
    bool LoadTextureFromFile(const char* filename, ID3D11ShaderResourceView** out_srv, 
        int* out_width, int* out_height, ID3D11Device* g_pd3dDevice);
    
};