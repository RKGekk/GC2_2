#include "texture_process.h"

int GetTextureIndex(aiString* pString) {
	return atoi(&pString->C_Str()[1]);
}

TextureStorageType DetermineTextureStorageType(const aiScene* pScene, aiMaterial* pMat, unsigned int index, aiTextureType textureType) {
	if (pMat->GetTextureCount(textureType) == 0) { return TextureStorageType::None; }

	aiString path;
	pMat->GetTexture(textureType, index, &path);
	std::string texturePath = path.C_Str();
	if (texturePath[0] == '*') {
		if (pScene->mTextures[0]->mHeight == 0) { return TextureStorageType::EmbeddedIndexCompressed; }
		else { return TextureStorageType::EmbeddedIndexNonCompressed; }
	}

	if (auto pTex = pScene->GetEmbeddedTexture(texturePath.c_str())) {
		if (pTex->mHeight == 0) { return TextureStorageType::EmbeddedCompressed; }
		else { return TextureStorageType::EmbeddedNonCompressed; }
	}

	return TextureStorageType::Disk;
}

Textures LoadMaterialTexures(ID3D11Device* device, aiMaterial* pMaterial, const aiScene* pScene) {
	Textures materialTextures;
	TextureStorageType storeType = TextureStorageType::Invalid;
	for (unsigned int i = 0u; i < aiTextureType::aiTextureType_UNKNOWN; ++i) {
		aiTextureType textureType = (aiTextureType)i;
		TextureType tt = GetTextureType(textureType);
		unsigned int textureCount = pMaterial->GetTextureCount(textureType);
		for (UINT i = 0u; i < textureCount; ++i) {
			aiString path;
			pMaterial->GetTexture(textureType, i, &path);
			TextureStorageType store_type = DetermineTextureStorageType(pScene, pMaterial, i, textureType);
			switch (store_type) {
			case TextureStorageType::EmbeddedIndexCompressed: {
				int index = GetTextureIndex(&path);
				materialTextures[tt].push_back(MaterialTexture(device, reinterpret_cast<uint8_t*>(pScene->mTextures[index]->pcData), pScene->mTextures[index]->mWidth, tt, -1));
			}
			break;
			case TextureStorageType::EmbeddedCompressed: {
				const aiTexture* pTexture = pScene->GetEmbeddedTexture(path.C_Str());
				materialTextures[tt].push_back(MaterialTexture(device, reinterpret_cast<uint8_t*>(pTexture->pcData), pTexture->mWidth, tt, -1));
			}
			break;
			case TextureStorageType::Disk: {
				std::string fileName = path.C_Str();
				materialTextures[tt].push_back(MaterialTexture(device, fileName, tt, -1));
			}
			break;
			}
		}
	}

	if (materialTextures.size() == 0u) {
		materialTextures.insert({ TextureType::DIFFUSE, { MaterialTexture(device, MaterialColors::UnhandeledTextureColor, TextureType::DIFFUSE, 0) } });
	}

	return materialTextures;
}

TextureType GetTextureType(aiTextureType aiType) {
	switch (aiType) {
		case aiTextureType_NONE: return TextureType::NONE;
		case aiTextureType_DIFFUSE: return TextureType::DIFFUSE;
		case aiTextureType_SPECULAR: return TextureType::SPECULAR;
		case aiTextureType_AMBIENT: return TextureType::AMBIENT;
		case aiTextureType_EMISSIVE: return TextureType::EMISSIVE;
		case aiTextureType_HEIGHT: return TextureType::HEIGHT;
		case aiTextureType_NORMALS: return TextureType::NORMALS;
		case aiTextureType_SHININESS: return TextureType::SHININESS;
		case aiTextureType_OPACITY: return TextureType::OPACITY;
		case aiTextureType_DISPLACEMENT: return TextureType::DISPLACEMENT;
		case aiTextureType_LIGHTMAP: return TextureType::LIGHTMAP;
		case aiTextureType_REFLECTION: return TextureType::REFLECTION;
		case aiTextureType_BASE_COLOR: return TextureType::BASE_COLOR;
		case aiTextureType_NORMAL_CAMERA: return TextureType::NORMAL_CAMERA;
		case aiTextureType_EMISSION_COLOR: return TextureType::EMISSION_COLOR;
		case aiTextureType_METALNESS: return TextureType::METALNESS;
		case aiTextureType_DIFFUSE_ROUGHNESS: return TextureType::DIFFUSE_ROUGHNESS;
		case aiTextureType_AMBIENT_OCCLUSION: return TextureType::AMBIENT_OCCLUSION;
		case aiTextureType_UNKNOWN: return TextureType::UNKNOWN;
		case _aiTextureType_Force32Bit: return TextureType::UNKNOWN;
		default: return TextureType::UNKNOWN;
	}
}
