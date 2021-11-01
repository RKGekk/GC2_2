#include "mesh_holder.h"
#include <utility>

MeshHolder::MeshHolder() {
	m_material = {
		{1.0f, 1.0f, 1.0f, 1.0f},
		{1.0f, 1.0f, 1.0f, 1.0f},
		{1.0f, 1.0f, 1.0f, 1.0f},
		{1.0f, 1.0f, 1.0f, 1.0f}
	};
}

MeshHolder::MeshHolder(Vertices vertices, Indices indices, Textures textures, MaterialShader material) {
	m_vertices = std::move(vertices);
	m_indices = std::move(indices);
	m_textures = std::move(textures);
	m_material = material;
}

MeshHolder::MeshHolder(MeshHolder&& right) {
	m_vertices = std::move(right.m_vertices);
	m_indices = std::move(right.m_indices);
	m_textures = std::move(right.m_textures);
	m_material = right.m_material;
}

MeshHolder::MeshHolder(const MeshHolder& right) {
	m_vertices = right.m_vertices;
	m_indices = right.m_indices;
	m_textures = right.m_textures;
	m_material = right.m_material;
}

MeshHolder& MeshHolder::operator=(const MeshHolder& right) {
	if (this == &right) { return *this;	}

	m_vertices = right.m_vertices;
	m_indices = right.m_indices;
	m_textures = right.m_textures;
	m_material = right.m_material;

	return *this;
}

MeshHolder& MeshHolder::operator=(MeshHolder&& right) {
	if (this == &right) { return *this; }
	m_vertices = std::move(right.m_vertices);
	m_indices = std::move(right.m_indices);
	m_textures = std::move(right.m_textures);
	m_material = right.m_material;
	return *this;
}

const Vertices& MeshHolder::GetVertices() const {
	return m_vertices;
}

void MeshHolder::SetVertices(Vertices v) {
	m_vertices = std::move(v);
}

const Indices& MeshHolder::GetIndices() const {
	return m_indices;
}

void MeshHolder::SetIndices(Indices i) {
	m_indices = std::move(i);
}

void MeshHolder::AddTexture(TextureType tt, MaterialTexture mt) {
	m_textures[tt].push_back(std::move(mt));
}

const Textures& MeshHolder::GetAllTextures() const {
	return m_textures;
}

const std::vector<MaterialTexture>& MeshHolder::GetTextures(TextureType t) const {
	return m_textures.at(t);
}

const MaterialTexture& MeshHolder::GetFirstTexture(TextureType t) const {
	return m_textures.at(t).at(0);
}

bool MeshHolder::CheckTexturePresence(TextureType t) const {
	return m_textures.count(t);
}

size_t MeshHolder::CountTextures(TextureType t) const {
	if (m_textures.count(t)) {
		m_textures.at(t);
	}
	else {
		return 0u;
	}
}

const MaterialShader& MeshHolder::GetMaterial() const {
	return m_material;
}