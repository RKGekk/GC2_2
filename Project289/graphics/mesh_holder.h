#pragma once

#include <vector>
#include <string>
#include <unordered_map>

#include <DirectXMath.h>

#include "../graphics/vertex.h"
#include "../graphics/material_texture.h"
#include "../graphics/material_shader.h"

//struct MeshHolder {
//    std::vector<Vertex> vertices;
//    std::vector<DWORD> indices;
//    std::vector<MaterialTexture> textures;
//    MaterialShader material;
//    DirectX::XMFLOAT4X4 transform;
//};

using Vertices = std::vector<Vertex>;
using Indices = std::vector<DWORD>;
using Textures = std::unordered_map<TextureType, std::vector<MaterialTexture>>;

class MeshHolder {
public:
    MeshHolder();
    MeshHolder(Vertices vertices, Indices indices, Textures textures, MaterialShader material);

    MeshHolder(MeshHolder&&);
    MeshHolder(const MeshHolder&);
    MeshHolder& operator=(const MeshHolder& right);
    MeshHolder& operator=(MeshHolder&& right);

    const Vertices& GetVertices() const;
    void SetVertices(Vertices v);

    const Indices& GetIndices() const;
    void SetIndices(Indices i);

    const MaterialShader& GetMaterial() const;

    void AddTexture(TextureType tt, MaterialTexture mt);
    const Textures& GetAllTextures() const;
    const std::vector<MaterialTexture>& GetTextures(TextureType t) const;
    const MaterialTexture& GetFirstTexture(TextureType t) const;
    bool CheckTexturePresence(TextureType t) const;
    size_t CountTextures(TextureType t) const;

private:
    Vertices m_vertices;
    Indices m_indices;
    Textures m_textures;
    MaterialShader m_material;
};