#pragma once

#include <string>

#include <assimp/material.h>
#include <assimp/scene.h>

#include "../graphics/material_texture.h"
#include "../graphics/mesh_holder.h"

TextureStorageType DetermineTextureStorageType(const aiScene* pScene, aiMaterial* pMat, unsigned int index, aiTextureType textureType);
int GetTextureIndex(aiString* pString);
Textures LoadMaterialTexures(ID3D11Device* device, aiMaterial* pMaterial, const aiScene* pScene);
TextureType GetTextureType(aiTextureType aiType);