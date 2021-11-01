#include "material_texture.h"

#include "../engine/engine.h"
#include "../engine/d3d_renderer11.h"

#include <algorithm>
#include <string>
#include <functional>
#include <cctype>
#include <utility>

std::unordered_map<std::string, std::pair<Microsoft::WRL::ComPtr<ID3D11Resource>, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>>> MaterialTexture::m_texture_cache;

MaterialTexture::MaterialTexture() : m_type(TextureType::UNKNOWN) {}

MaterialTexture::MaterialTexture(ID3D11Device* device, const std::string& fileName, int mip_levels) : MaterialTexture(device, fileName, TextureType::UNKNOWN, mip_levels){}

MaterialTexture::MaterialTexture(ID3D11Device* device, const MaterialColor& color, TextureType type, int mip_levels) {
	InitializeColorMaterial(device, color, type, mip_levels);
}

MaterialTexture::MaterialTexture(ID3D11Device* device, const MaterialColor* color_data, UINT width, UINT height, TextureType type, int mip_levels) {
	InitializeTextureMaterial(device, color_data, width, height, type, mip_levels);
}

MaterialTexture::MaterialTexture(ID3D11Device* device, const std::string& fileName, TextureType type, int mip_levels) : m_type(type) {

	std::filesystem::path p(fileName);
	std::error_code err;
	std::filesystem::file_status status = std::filesystem::status(p, err);
	if (!std::filesystem::is_regular_file(status)) {
		InitializeColorMaterial(device, MaterialColors::UnloadedTextureColor, type, mip_levels);
	}
	std::string upper_ext = p.extension().string();
	std::for_each(upper_ext.begin(), upper_ext.end(), [](char& c) { c = ::toupper(c); });
	if (upper_ext == ".DDS") {
		if (m_texture_cache.count(fileName)) {
			auto& tp = m_texture_cache[fileName];
			m_texture = tp.first;
			m_texture_view = tp.second;
		}
		else {
			D3DRenderer11* renderer = static_cast<D3DRenderer11*>(g_pApp->GetRenderer());
			//HRESULT hr = DirectX::CreateDDSTextureFromFile(device, s2w(fileName).c_str(), m_texture.GetAddressOf(), m_texture_view.GetAddressOf());
			HRESULT hr = DirectX::CreateDDSTextureFromFileEx(
				device,
				renderer->GetDeviceContext(),
				s2w(fileName).c_str(),
				0,
				D3D11_USAGE_DEFAULT,
				D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE,
				0,
				D3D11_RESOURCE_MISC_GENERATE_MIPS,
				false,
				m_texture.GetAddressOf(),
				m_texture_view.GetAddressOf()
			);
			if (FAILED(hr)) {
				InitializeColorMaterial(device, MaterialColors::UnloadedTextureColor, type, mip_levels);
			}
			else {
				m_texture_cache[fileName] = { m_texture, m_texture_view };
			}
		}
	}
	else {
		if (m_texture_cache.count(fileName)) {
			auto& tp = m_texture_cache[fileName];
			m_texture = tp.first;
			m_texture_view = tp.second;
		}
		else {
			//HRESULT hr = DirectX::CreateWICTextureFromFile(device, s2w(fileName).c_str(), m_texture.GetAddressOf(), m_texture_view.GetAddressOf());
			D3DRenderer11* renderer = static_cast<D3DRenderer11*>(g_pApp->GetRenderer());
			HRESULT hr = DirectX::CreateWICTextureFromFileEx(
				device,
				renderer->GetDeviceContext(),
				s2w(fileName).c_str(),
				0,
				D3D11_USAGE_DEFAULT,
				D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE,
				0,
				D3D11_RESOURCE_MISC_GENERATE_MIPS,
				DirectX::WIC_LOADER_DEFAULT,
				m_texture.GetAddressOf(),
				m_texture_view.GetAddressOf()
			);
			if (FAILED(hr)) {
				InitializeColorMaterial(device, MaterialColors::UnloadedTextureColor, type, mip_levels);
			}
			else {
				m_texture_cache[fileName] = { m_texture, m_texture_view };
				/*if (mip_levels) {
					D3D11_SHADER_RESOURCE_VIEW_DESC texDesc;
					m_texture_view->GetDesc(&texDesc);
					texDesc.Texture2D.MostDetailedMip = 0;
					texDesc.Texture2D.MipLevels = mip_levels;
					D3DRenderer11* renderer = static_cast<D3DRenderer11*>(g_pApp->GetRenderer());
					renderer->GetDeviceContext()->GenerateMips(m_texture_view.Get());
				}*/
			}
		}
	}
}

MaterialTexture::MaterialTexture(ID3D11Device* device, const uint8_t* pData, size_t size, TextureType type, int mip_levels) {
	m_type = type;
	HRESULT hr = DirectX::CreateWICTextureFromMemory(device, pData, size, m_texture.GetAddressOf(), m_texture_view.GetAddressOf());
	COM_ERROR_IF_FAILED(hr, "Failed create texture from memory.");
}

MaterialTexture::MaterialTexture(const MaterialTexture& other) {
	m_texture = other.m_texture;
	m_texture_view = other.m_texture_view;
	m_type = other.m_type;
}

MaterialTexture::MaterialTexture(MaterialTexture&& other) {
	m_texture = other.m_texture;
	m_texture_view = other.m_texture_view;
	m_type = other.m_type;
}

MaterialTexture& MaterialTexture::operator=(const MaterialTexture& right) {
	if (this == &right) { return *this; }
	m_texture = right.m_texture;
	m_texture_view = right.m_texture_view;
	m_type = right.m_type;
	return *this;
}

TextureType MaterialTexture::GetType() {
	return m_type;
}

ID3D11ShaderResourceView* MaterialTexture::GetTextureResourceView() {
	return m_texture_view.Get();
}

ID3D11ShaderResourceView* const* MaterialTexture::GetTextureResourceViewAddress() const {
	return m_texture_view.GetAddressOf();
}

void MaterialTexture::InitializeColorMaterial(ID3D11Device* device, const MaterialColor& color, TextureType type, int mip_levels) {
	InitializeTextureMaterial(device, &color, 1, 1, type, mip_levels);
}

void MaterialTexture::InitializeTextureMaterial(ID3D11Device* device, const MaterialColor* color_data, UINT width, UINT height, TextureType type, int mip_levels) {
	m_type = type;
	if (m_texture != nullptr) { m_texture.Reset(); }

	D3D11_TEXTURE2D_DESC textureDesc;
	ZeroMemory(&textureDesc, sizeof(textureDesc));
	textureDesc.Width = width;
	textureDesc.Height = height;
	if (mip_levels) {
		mip_levels = mip_levels;
	}
	else {
		textureDesc.MipLevels = 1;
	}
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;
	if (mip_levels) {
		textureDesc.MipLevels = mip_levels;
		textureDesc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;
	}
	else {
		textureDesc.MipLevels = 1;
		textureDesc.MiscFlags = 0;
	}

	D3D11_SUBRESOURCE_DATA init_data;
	ZeroMemory(&init_data, sizeof(init_data));
	init_data.pSysMem = color_data;
	init_data.SysMemPitch = width * sizeof(MaterialColor);
	HRESULT hr = device->CreateTexture2D(&textureDesc, &init_data, reinterpret_cast<ID3D11Texture2D**>(m_texture.GetAddressOf()));
	COM_ERROR_IF_FAILED(hr, "Failed to initialize texture from color data.");

	CD3D11_SHADER_RESOURCE_VIEW_DESC srvDesc(D3D11_SRV_DIMENSION_TEXTURE2D, DXGI_FORMAT_R8G8B8A8_UNORM);
	if (mip_levels) {
		srvDesc.Texture2D.MostDetailedMip = 0;
		srvDesc.Texture2D.MipLevels = mip_levels;
	}
	hr = device->CreateShaderResourceView(m_texture.Get(), &srvDesc, m_texture_view.ReleaseAndGetAddressOf());
	COM_ERROR_IF_FAILED(hr, "Failed to create shader resource view from color data.");

	if (mip_levels) {
		D3DRenderer11* renderer = static_cast<D3DRenderer11*>(g_pApp->GetRenderer());
		renderer->GetDeviceContext()->GenerateMips(m_texture_view.Get());
	}
}