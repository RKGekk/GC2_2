#include "mesh_render_light_component.h"

#include "transform_component.h"
#include "mesh_component.h"
#include "../tools/memory_utility.h"
#include "../tools/math_utitity.h"
//#include "../nodes/d3d_11_mesh.h"
#include "../nodes/d3d_11_light_pipeline.h"
#include "../engine/engine.h"
#include "../engine/d3d_renderer11.h"
#include "particle_component.h"
#include "../tools/texture_process.h"

#include <unordered_map>

const std::string MeshRenderLightComponent::g_Name = "MeshRenderLightComponent";
int MeshRenderLightComponent::m_last_mesh_id = 0;

const std::string& MeshRenderLightComponent::VGetName() const {
	return g_Name;
}

MeshRenderLightComponent::MeshRenderLightComponent() : m_auto_radius(false), m_radius(0.0f), m_alpha_blend(false), m_diffuse_texture_address_mode_u(D3D11_TEXTURE_ADDRESS_WRAP), m_diffuse_texture_address_mode_v(D3D11_TEXTURE_ADDRESS_WRAP), m_scale(1.0f) {}

const std::string& MeshRenderLightComponent::GetPixelShaderResource() const {
	return m_pixelShaderResource;
}

const std::string& MeshRenderLightComponent::GetVertexShaderResource() const {
	return m_vertexShaderResource;
}

const std::vector<D3D11_INPUT_ELEMENT_DESC>& MeshRenderLightComponent::GetLayout() const {
	return m_vs_layout;
}

const MeshHolder& MeshRenderLightComponent::GetMesh(int key) const {
	return m_meshes.at(key);
}

bool MeshRenderLightComponent::GetAlphaBlend() const {
	return m_alpha_blend;
}

D3D11_TEXTURE_ADDRESS_MODE MeshRenderLightComponent::GetDiffuseTextureAddressModeU() const {
	return m_diffuse_texture_address_mode_u;
}

D3D11_TEXTURE_ADDRESS_MODE MeshRenderLightComponent::GetDiffuseTextureAddressModeV() const {
	return m_diffuse_texture_address_mode_v;
}

float MeshRenderLightComponent::GetFogStart() const {
	return m_fog_start;
}

float MeshRenderLightComponent::GetFogRange() const {
	return m_fog_range;
}

DirectX::XMFLOAT4 MeshRenderLightComponent::GetFogColor() const {
	return m_fog_color;
}

bool MeshRenderLightComponent::VDelegateInit(TiXmlElement* pData) {

	TiXmlElement* pPixelShader = pData->FirstChildElement("PixelShader");
	if (pPixelShader) {
		m_pixelShaderResource = pPixelShader->FirstChild()->Value();
	}

	TiXmlElement* pVertexShader = pData->FirstChildElement("VertexShader");
	if (pVertexShader) {
		m_vertexShaderResource = pVertexShader->FirstChild()->Value();
	}

	TiXmlElement* pRadius = pData->FirstChildElement("Radius");
	if (pRadius) {
		std::string radius_str = pRadius->FirstChild()->Value();
		if (radius_str == "auto" || radius_str == "Auto" || radius_str == "AUTO") {
			m_auto_radius = true;
		}
		else {
			float radius = std::stof(radius_str);
			m_radius = radius;
		}
	}

	TiXmlElement* pDiffuseTextureAddressModeV = pData->FirstChildElement("DiffuseTextureAddressModeV");
	if (pDiffuseTextureAddressModeV) {
		std::string address_mode_str = pDiffuseTextureAddressModeV->FirstChild()->Value();
		std::for_each(address_mode_str.begin(), address_mode_str.end(), [](char& c) { c = ::toupper(c); });
		if (address_mode_str == "MIRROR") {
			m_diffuse_texture_address_mode_v = D3D11_TEXTURE_ADDRESS_MIRROR;
		}
		else if (address_mode_str == "CLAMP") {
			m_diffuse_texture_address_mode_v = D3D11_TEXTURE_ADDRESS_CLAMP;
		}
		else if (address_mode_str == "BORDER") {
			m_diffuse_texture_address_mode_v = D3D11_TEXTURE_ADDRESS_BORDER;
		}
		else if (address_mode_str == "MIRROR_ONCE") {
			m_diffuse_texture_address_mode_v = D3D11_TEXTURE_ADDRESS_MIRROR_ONCE;
		}
		else {
			m_diffuse_texture_address_mode_v = D3D11_TEXTURE_ADDRESS_WRAP;
		}
	}

	TiXmlElement* pDiffuseTextureAddressModeU = pData->FirstChildElement("DiffuseTextureAddressModeU");
	if (pDiffuseTextureAddressModeU) {
		std::string address_mode_str = pDiffuseTextureAddressModeU->FirstChild()->Value();
		std::for_each(address_mode_str.begin(), address_mode_str.end(), [](char& c) { c = ::toupper(c); });
		if (address_mode_str == "MIRROR") {
			m_diffuse_texture_address_mode_u = D3D11_TEXTURE_ADDRESS_MIRROR;
		}
		else if (address_mode_str == "CLAMP") {
			m_diffuse_texture_address_mode_u = D3D11_TEXTURE_ADDRESS_CLAMP;
		}
		else if (address_mode_str == "BORDER") {
			m_diffuse_texture_address_mode_u = D3D11_TEXTURE_ADDRESS_BORDER;
		}
		else if (address_mode_str == "MIRROR_ONCE") {
			m_diffuse_texture_address_mode_u = D3D11_TEXTURE_ADDRESS_MIRROR_ONCE;
		}
		else {
			m_diffuse_texture_address_mode_u = D3D11_TEXTURE_ADDRESS_WRAP;
		}
	}

	TiXmlElement* pAlphaBlend = pData->FirstChildElement("AlphaBlend");
	if (pAlphaBlend) {
		std::string alpha_blend_str = pAlphaBlend->FirstChild()->Value();
		if (alpha_blend_str == "true" || alpha_blend_str == "1" || alpha_blend_str == "TRUE") {
			m_alpha_blend = true;
		}
	}

	TiXmlElement* pFogStart = pData->FirstChildElement("FogStart");
	if (pFogStart) {
		std::string fog_start_str = pFogStart->FirstChild()->Value();
		float fog_start = std::stof(fog_start_str);
		m_fog_start = fog_start;
	}

	TiXmlElement* pFogRange = pData->FirstChildElement("FogRange");
	if (pFogRange) {
		std::string fog_range_str = pFogRange->FirstChild()->Value();
		float fog_range = std::stof(fog_range_str);
		m_fog_range = fog_range;
	}

	TiXmlElement* pFogColor = pData->FirstChildElement("FogColor");
	if (pFogColor) {
		DirectX::XMFLOAT4 color = LoadColor(pFogColor);
		m_fog_color.x = color.x;
		m_fog_color.y = color.y;
		m_fog_color.z = color.z;
		m_fog_color.w = color.w;
	}

	float m_fog_range;
	DirectX::XMFLOAT4 m_fog_color;

	m_vs_layout = {
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

	return true;
}

std::shared_ptr<SceneNode> MeshRenderLightComponent::VCreateSceneNode() {
	std::shared_ptr<TransformComponent> pTransformComponent = MakeStrongPtr(m_pOwner->GetComponent<TransformComponent>(TransformComponent::g_Name));
	std::shared_ptr<MeshComponent> pMeshComponent = MakeStrongPtr(m_pOwner->GetComponent<MeshComponent>(MeshComponent::g_Name));
	if (!pTransformComponent || !pMeshComponent) { return std::shared_ptr<SceneNode>(); }

	m_resource_directory = pMeshComponent->GetResourceDirecory();

	DirectX::XMFLOAT3 xm_scale = pTransformComponent->GetScale3f();
	float scale = xm_scale.x;
	scale = scale > xm_scale.y ? scale : xm_scale.y;
	scale = scale > xm_scale.z ? scale : xm_scale.z;
	m_scale = scale;
	m_scale3f = xm_scale;

	std::shared_ptr<SceneNode> root_node(new SceneNode(this, RenderPass::RenderPass_Actor, &pTransformComponent->GetTransform4x4f()));
	const aiScene* pScene = pMeshComponent->GetScene();
	ProcessNode(pScene->mRootNode, pScene, root_node);

	std::shared_ptr<ParticleComponent> pParticleComponent = MakeStrongPtr(m_pOwner->GetComponent<ParticleComponent>(ParticleComponent::g_Name));
	if (pParticleComponent && !m_auto_radius) {
		root_node->SetRadius(pParticleComponent->VGetParticle().getRadius());
	}

	return root_node;
}

void MeshRenderLightComponent::VCreateInheritedXmlElements(TiXmlElement* pBaseElement) {}

void MeshRenderLightComponent::ProcessNode(aiNode* node, const aiScene* scene, std::shared_ptr<SceneNode> parent) {
	using namespace DirectX;
	XMMATRIX node_transform_matrix = XMMatrixTranspose(XMMATRIX(&node->mTransformation.a1));

	for (UINT i = 0; i < node->mNumMeshes; ++i) {
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		parent->VAddChild(ProcessMesh(mesh, scene, node_transform_matrix));
	}
	for (UINT i = 0; i < node->mNumChildren; ++i) {
		ActorId owner = this->m_pOwner->GetId();
		std::shared_ptr<SceneNode> n = std::make_shared<SceneNode>(nullptr, RenderPass::RenderPass_Actor, node_transform_matrix, DirectX::XMMatrixIdentity(), true);

		parent->VAddChild(n);
		ProcessNode(node->mChildren[i], scene, n);
	}
}

std::shared_ptr<SceneNode> MeshRenderLightComponent::ProcessMesh(aiMesh* mesh, const aiScene* scene, DirectX::FXMMATRIX nodeMatrix) {
	std::vector<Vertex> vertices;
	std::vector<DWORD> indices;

	float max_x = m_min; float max_y = m_min; float max_z = m_min;
	float min_x = m_max; float min_y = m_max; float min_z = m_max;
	for (UINT i = 0; i < mesh->mNumVertices; ++i) {
		Vertex vertex;
		vertex.color = { 0.7f, 0.1f, 0.6f, 1.0f };
		vertex.pos = { mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z };
		vertex.normal = { mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z };
		vertex.tg = { mesh->mTangents[i].x, mesh->mTangents[i].y, mesh->mTangents[i].z };
		if (mesh->mTextureCoords[0]) {
			vertex.uv = { (float)mesh->mTextureCoords[0][i].x, (float)mesh->mTextureCoords[0][i].y };
		}

		if (vertex.pos.x > max_x) { max_x = vertex.pos.x; }
		if (vertex.pos.y > max_y) { max_y = vertex.pos.y; }
		if (vertex.pos.z > max_z) { max_z = vertex.pos.z; }

		if (vertex.pos.x < min_x) { min_x = vertex.pos.x; }
		if (vertex.pos.y < min_y) { min_y = vertex.pos.y; }
		if (vertex.pos.z < min_z) { min_z = vertex.pos.z; }

		vertices.push_back(vertex);
	}
	if (max_x > m_max_x) { m_max_x = max_x; }
	if (max_y > m_max_y) { m_max_y = max_y; }
	if (max_z > m_max_z) { m_max_z = max_z; }

	if (min_x < m_min_x) { m_min_x = min_x; }
	if (min_y < m_min_y) { m_min_y = min_y; }
	if (min_z < m_min_z) { m_min_z = min_z; }

	for (UINT i = 0; i < mesh->mNumFaces; ++i) {
		aiFace face = mesh->mFaces[i];
		for (UINT j = 0; j < face.mNumIndices; ++j) {
			indices.push_back(face.mIndices[j]);
		}
	}

	aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
	D3DRenderer11* renderer = static_cast<D3DRenderer11*>(g_pApp->GetRenderer());

	Textures textures = LoadMaterialTexures(renderer->GetDevice(), material, scene);

	DirectX::XMFLOAT4X4 nodeTransformMatrix4x4f;
	DirectX::XMStoreFloat4x4(&nodeTransformMatrix4x4f, nodeMatrix);

	MaterialShader mt;
	aiColor3D aiColor(0.0f, 0.0f, 0.0f);
	material->Get(AI_MATKEY_COLOR_DIFFUSE, aiColor);
	mt.Diffuse = { aiColor.r, aiColor.g, aiColor.b, 1.0f };
	material->Get(AI_MATKEY_COLOR_AMBIENT, aiColor);
	mt.Ambient = { aiColor.r, aiColor.g, aiColor.b, 1.0f };
	material->Get(AI_MATKEY_COLOR_SPECULAR, aiColor);
	mt.Specular = { aiColor.r, aiColor.g, aiColor.b, 1.0f };

	m_meshes[++m_last_mesh_id] = MeshHolder(std::move(vertices), std::move(indices), std::move(textures), mt);

	std::shared_ptr<D3D11LightPipeline> result = std::make_shared<D3D11LightPipeline>(m_last_mesh_id, *this, nodeMatrix, DirectX::XMMatrixIdentity(), true);
	result->SetAlpha(GetColor().w);
	if (m_auto_radius) {
		float x = std::fabsf(max_x) > std::fabsf(min_x) ? std::fabsf(max_x) : std::fabsf(min_x);
		float y = std::fabsf(max_y) > std::fabsf(min_y) ? std::fabsf(max_y) : std::fabsf(min_y);
		float z = std::fabsf(max_z) > std::fabsf(min_z) ? std::fabsf(max_z) : std::fabsf(min_z);
		float radius = DirectX::XMVectorGetX(DirectX::XMVector3Length(DirectX::XMVectorSet(x, y, z, 0.0f)));
		result->SetRadius(radius * m_scale);
	}
	return result;
}

DirectX::XMMATRIX MeshRenderLightComponent::InverseTranspose(DirectX::CXMMATRIX M) {
	DirectX::XMMATRIX A = M;
	A.r[3] = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
	DirectX::XMVECTOR det = XMMatrixDeterminant(A);
	return XMMatrixTranspose(XMMatrixInverse(&det, A));;
}

DirectX::XMMATRIX MeshRenderLightComponent::Inverse(DirectX::CXMMATRIX M) {
	DirectX::XMMATRIX A = M;
	A.r[3] = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
	DirectX::XMVECTOR det = XMMatrixDeterminant(A);
	return XMMatrixInverse(&det, A);
}