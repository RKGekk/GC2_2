#pragma once

#include <string>
#include <memory>
#include <utility>
#include <vector>
#include <unordered_map>

#include <DirectXMath.h>

#include <assimp/material.h>
#include <assimp/scene.h>
#include <WICTextureLoader.h>
#include <DDSTextureLoader.h>

#include "base_render_component.h"
#include "../bindable/vertex_constant_buffer_bindable.h"
#include "../bindable/pixel_constant_buffer_bindable.h"
#include "../graphics/material_texture.h"
#include "../graphics/constant_buffer_types.h"
#include "../graphics/vertex.h"
#include "../graphics/mesh_holder.h"

class MeshRenderLightComponent : public BaseRenderComponent {
    std::string m_pixelShaderResource;
    std::string m_vertexShaderResource;
    std::string m_resource_directory;

    std::vector<D3D11_INPUT_ELEMENT_DESC> m_vs_layout;
    std::unordered_map<int, MeshHolder> m_meshes;
    static int m_last_mesh_id;
    bool m_auto_radius;
    float m_radius;
    bool m_alpha_blend;
    D3D11_TEXTURE_ADDRESS_MODE m_diffuse_texture_address_mode_u;
    D3D11_TEXTURE_ADDRESS_MODE m_diffuse_texture_address_mode_v;

    static constexpr float m_max = std::numeric_limits<float>::max();
    static constexpr float m_min = std::numeric_limits<float>::min();
    float m_max_x = m_min; float m_max_y = m_min; float m_max_z = m_min;
    float m_min_x = m_max; float m_min_y = m_max; float m_min_z = m_max;

    float m_fog_start;
    float m_fog_range;
    DirectX::XMFLOAT4 m_fog_color;
    float m_scale;
    DirectX::XMFLOAT3 m_scale3f;

public:
    static const std::string g_Name;
    virtual const std::string& VGetName() const;

    MeshRenderLightComponent();

    const std::string& GetPixelShaderResource() const;
    const std::string& GetVertexShaderResource() const;
    const std::vector<D3D11_INPUT_ELEMENT_DESC>& GetLayout() const;
    const MeshHolder& GetMesh(int key) const;
    bool GetAlphaBlend() const;
    D3D11_TEXTURE_ADDRESS_MODE GetDiffuseTextureAddressModeU() const;
    D3D11_TEXTURE_ADDRESS_MODE GetDiffuseTextureAddressModeV() const;
    float GetFogStart() const;
    float GetFogRange() const;
    DirectX::XMFLOAT4 GetFogColor() const;

protected:
    virtual bool VDelegateInit(TiXmlElement* pData) override;
    virtual std::shared_ptr<SceneNode> VCreateSceneNode() override;

    virtual void VCreateInheritedXmlElements(TiXmlElement* pBaseElement) override;

    static DirectX::XMMATRIX InverseTranspose(DirectX::CXMMATRIX M);
    static DirectX::XMMATRIX Inverse(DirectX::CXMMATRIX M);

    void ProcessNode(aiNode* node, const aiScene* scene, std::shared_ptr<SceneNode> parent);
    std::shared_ptr<SceneNode> ProcessMesh(aiMesh* mesh, const aiScene* scene, DirectX::FXMMATRIX nodeMatrix);
};