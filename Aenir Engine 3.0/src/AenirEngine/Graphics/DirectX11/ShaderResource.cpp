#include"PCH.h"
#include "ShaderResource.h"
#define STB_IMAGE_IMPLEMENTATION
#include"..\..\ThirdParty\stb_image.h"

namespace Aen {

    ShaderResource::~ShaderResource() {
        m_srv.Reset();
    }

    ShaderResource::ShaderResource()
        :m_srv(NULL) {}

    void ShaderResource::LoadResource(const std::string& dir) {

        stbi_set_flip_vertically_on_load(true);
            int width = 2048, height = 2048, channels = 4;
            unsigned char* image = stbi_load(dir.c_str(), &width, &height, &channels, STBI_rgb_alpha);

            std::vector<unsigned char> tData;
            tData.resize(width * height * 4);

            for(int i = 0; i < height; i++) {
                for(int j = 0; j < width; j++) {
                    unsigned int pos0 = j * 4 + width * 4 * i;
                    tData[pos0] = image[pos0];
                    tData[pos0 + 1] = image[pos0 + 1];
                    tData[pos0 + 2] = image[pos0 + 2];
                    tData[pos0 + 3] = image[pos0 + 3];
                }
            }

            ComTexture2D uvTexture;
            D3D11_SUBRESOURCE_DATA sData;
            ZeroMemory(&sData, sizeof(D3D11_SUBRESOURCE_DATA));

            sData.pSysMem = tData.data();
            sData.SysMemPitch = width * 4;
            sData.SysMemSlicePitch = 0;

            D3D11_TEXTURE2D_DESC texDesc;
            ZeroMemory(&texDesc, sizeof(D3D11_TEXTURE2D_DESC));

            texDesc.Width = width;
            texDesc.Height = height;
            texDesc.MipLevels = 1;
            texDesc.ArraySize = 1;
            texDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
            texDesc.SampleDesc.Count = 1;
            texDesc.SampleDesc.Quality = 0;
            texDesc.Usage = D3D11_USAGE_IMMUTABLE;
            texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
            texDesc.CPUAccessFlags = 0;
            texDesc.MiscFlags = 0;

            if(FAILED(m_device->CreateTexture2D(&texDesc, &sData, uvTexture.GetAddressOf())))
                throw;

            if(FAILED(m_device->CreateShaderResourceView(uvTexture.Get(), nullptr, m_srv.GetAddressOf())))
                throw;

            stbi_image_free(image);
            uvTexture.Reset();
    }
}