#pragma once

#include <d3d11.h>
#include <wrl.h>
#include <filesystem>
#include <unordered_map>
#include <string>
#include <utility>

#include <WICTextureLoader.h>
#include <DDSTextureLoader.h>

#include "material_color.h"
#include "../tools/error_logger.h"
#include "../tools/string_utility.h"

enum class TextureStorageType {
	Invalid,
	None,
	EmbeddedIndexCompressed,
	EmbeddedIndexNonCompressed,
	EmbeddedCompressed,
	EmbeddedNonCompressed,
	Disk
};

enum class TextureType {
	NONE = 0,
	DIFFUSE = 1,
	SPECULAR = 2,
	AMBIENT = 3,
	EMISSIVE = 4,
	HEIGHT = 5,
	NORMALS = 6,
	SHININESS = 7,
	OPACITY = 8,
	DISPLACEMENT = 9,
	LIGHTMAP = 10,
	REFLECTION = 11,
	BASE_COLOR = 12,
	NORMAL_CAMERA = 13,
	EMISSION_COLOR = 14,
	METALNESS = 15,
	DIFFUSE_ROUGHNESS = 16,
	AMBIENT_OCCLUSION = 17,
	UNKNOWN = 18,
	SHADOWMAP = 19
};

using ResourcePair = std::pair<Microsoft::WRL::ComPtr<ID3D11Resource>, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>>;

class MaterialTexture {
public:
	MaterialTexture();
	MaterialTexture(ID3D11Device* device, const std::string& fileName, int mip_levels);
	MaterialTexture(ID3D11Device* device, const MaterialColor& color, TextureType type, int mip_levels);
	MaterialTexture(ID3D11Device* device, const MaterialColor* color_data, UINT width, UINT height, TextureType type, int mip_levels);
	MaterialTexture(ID3D11Device* device, const std::string& fileName, TextureType type, int mip_levels);
	MaterialTexture(ID3D11Device* device, const uint8_t* pData, size_t size, TextureType type, int mip_levels);

	MaterialTexture(const MaterialTexture& other);
	MaterialTexture(MaterialTexture&& other);
	MaterialTexture& operator=(const MaterialTexture& right);

	TextureType GetType();
	ID3D11ShaderResourceView* GetTextureResourceView();
	ID3D11ShaderResourceView* const* GetTextureResourceViewAddress() const;

private:
	void InitializeColorMaterial(ID3D11Device* device, const MaterialColor& color, TextureType type, int mip_levels);
	void InitializeTextureMaterial(ID3D11Device* device, const MaterialColor* color_data, UINT width, UINT height, TextureType type, int mip_levels);

	Microsoft::WRL::ComPtr<ID3D11Resource> m_texture;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texture_view;

	static std::unordered_map<std::string, ResourcePair> m_texture_cache;
	

	TextureType m_type;
};