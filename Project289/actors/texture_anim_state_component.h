#pragma once

#pragma once

#include <filesystem>
#include <string>
#include <unordered_map>
#include <vector>
#include <algorithm>

#include <DirectXMath.h>

#include "actor_component.h"
#include "pers_current_state_enum.h"
#include "../tools/string_utility.h"

using namespace std::literals;

struct FrameData {
	bool VerticalFlip;
	bool HorizontalFlip;
	std::vector<int> Data;
	float FrameTime;
};

class TextureAnimStateComponent : public ActorComponent {
public:
	static const std::string g_Name;
	virtual const std::string& VGetName() const override;

	TextureAnimStateComponent();
	~TextureAnimStateComponent();

	virtual bool VInit(TiXmlElement* pData) override;
	virtual void VPostInit() override;
	virtual void VUpdate(float deltaMs) override;

	virtual DirectX::XMFLOAT4X4 GetTexTransform4x4();
	virtual DirectX::XMFLOAT4X4 GetTexTransform4x4T();
	virtual PersCurrentStateEnum GetState();
	virtual PersCurrentStateClassEnum GetStateClass();
	virtual PersCurrentOrientClassEnum GetOrientClass();
	virtual void SetState(PersCurrentStateEnum state);

	virtual TiXmlElement* VGenerateXml() override;

protected:
	DirectX::XMFLOAT4X4 m_text_transform;
	float m_anim_time;
	int m_atlas_width;
	int m_atlas_height;
	std::string m_atlas_name;
	float m_total_time;
	PersCurrentStateEnum m_current_state;

	std::unordered_map<PersCurrentStateEnum, FrameData> m_frames_data;
};