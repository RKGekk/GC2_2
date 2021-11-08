#include "texture_anim_state_component.h"
#include "particle_component.h"
#include "../tools/memory_utility.h"

#include <cmath>

const std::string TextureAnimStateComponent::g_Name = "TextureAnimStateComponent"s;

const std::string& TextureAnimStateComponent::VGetName() const {
	return TextureAnimStateComponent::g_Name;
}

TextureAnimStateComponent::TextureAnimStateComponent() {
	DirectX::XMStoreFloat4x4(&m_tex_transform, DirectX::XMMatrixIdentity());
	m_anim_time = 0.0f;
	m_current_state = PersCurrentStateEnum::IdleToward;

	m_atlas_width = 1;
	m_atlas_height = 1;

	m_total_time = 0.0f;

	FrameData frame_data{ false, false, {0}, 0.1f };
	m_frames_data[PersCurrentStateEnum::WalkLeft] = frame_data;
	m_frames_data[PersCurrentStateEnum::WalkRight] = frame_data;
	m_frames_data[PersCurrentStateEnum::WalkToward] = frame_data;
	m_frames_data[PersCurrentStateEnum::WalkOutward] = frame_data;
	m_frames_data[PersCurrentStateEnum::WalkLeftToward] = frame_data;
	m_frames_data[PersCurrentStateEnum::WalkRightToward] = frame_data;
	m_frames_data[PersCurrentStateEnum::WalkLeftOutward] = frame_data;
	m_frames_data[PersCurrentStateEnum::WalkRightOutward] = frame_data;
	m_frames_data[PersCurrentStateEnum::JumpLeft] = frame_data;
	m_frames_data[PersCurrentStateEnum::JumpRight] = frame_data;
	m_frames_data[PersCurrentStateEnum::JumpToward] = frame_data;
	m_frames_data[PersCurrentStateEnum::JumpOutward] = frame_data;
	m_frames_data[PersCurrentStateEnum::JumpLeftToward] = frame_data;
	m_frames_data[PersCurrentStateEnum::JumpRightToward] = frame_data;
	m_frames_data[PersCurrentStateEnum::JumpLeftOutward] = frame_data;
	m_frames_data[PersCurrentStateEnum::JumpRightOutward] = frame_data;
	m_frames_data[PersCurrentStateEnum::IdleLeft] = frame_data;
	m_frames_data[PersCurrentStateEnum::IdleRight] = frame_data;
	m_frames_data[PersCurrentStateEnum::IdleToward] = frame_data;
	m_frames_data[PersCurrentStateEnum::IdleOutward] = frame_data;
	m_frames_data[PersCurrentStateEnum::IdleLeftToward] = frame_data;
	m_frames_data[PersCurrentStateEnum::IdleRightToward] = frame_data;
	m_frames_data[PersCurrentStateEnum::IdleLeftOutward] = frame_data;
	m_frames_data[PersCurrentStateEnum::IdleRightOutward] = frame_data;
	m_frames_data[PersCurrentStateEnum::FireLeft] = frame_data;
	m_frames_data[PersCurrentStateEnum::FireRight] = frame_data;
	m_frames_data[PersCurrentStateEnum::FireToward] = frame_data;
	m_frames_data[PersCurrentStateEnum::FireOutward] = frame_data;
	m_frames_data[PersCurrentStateEnum::FireLeftToward] = frame_data;
	m_frames_data[PersCurrentStateEnum::FireRightToward] = frame_data;
	m_frames_data[PersCurrentStateEnum::FireLeftOutward] = frame_data;
	m_frames_data[PersCurrentStateEnum::FireRightOutward] = frame_data;
	m_frames_data[PersCurrentStateEnum::DieLeft] = frame_data;
	m_frames_data[PersCurrentStateEnum::DieRight] = frame_data;
	m_frames_data[PersCurrentStateEnum::DieToward] = frame_data;
	m_frames_data[PersCurrentStateEnum::DieOutward] = frame_data;
	m_frames_data[PersCurrentStateEnum::DieLeftToward] = frame_data;
	m_frames_data[PersCurrentStateEnum::DieRightToward] = frame_data;
	m_frames_data[PersCurrentStateEnum::DieLeftOutward] = frame_data;
	m_frames_data[PersCurrentStateEnum::DieRightOutward] = frame_data;
}

TextureAnimStateComponent::~TextureAnimStateComponent() {}

PersCurrentStateEnum decodeStateEnum(const std::string& sType) {
	if (sType == "WalkLeft"s) return PersCurrentStateEnum::WalkLeft;
	if (sType == "WalkRight"s) return PersCurrentStateEnum::WalkRight;
	if (sType == "WalkToward"s) return PersCurrentStateEnum::WalkToward;
	if (sType == "WalkOutward"s) return PersCurrentStateEnum::WalkOutward;
	if (sType == "WalkLeftToward"s) return PersCurrentStateEnum::WalkLeftToward;
	if (sType == "WalkRightToward"s) return PersCurrentStateEnum::WalkRightToward;
	if (sType == "WalkLeftOutward"s) return PersCurrentStateEnum::WalkLeftOutward;
	if (sType == "JumpLeft"s) return PersCurrentStateEnum::JumpLeft;
	if (sType == "JumpRight"s) return PersCurrentStateEnum::JumpRight;
	if (sType == "JumpToward"s) return PersCurrentStateEnum::JumpToward;
	if (sType == "JumpOutward"s) return PersCurrentStateEnum::JumpOutward;
	if (sType == "JumpLeftToward"s) return PersCurrentStateEnum::JumpLeftToward;
	if (sType == "JumpRightToward"s) return PersCurrentStateEnum::JumpRightToward;
	if (sType == "JumpLeftOutward"s) return PersCurrentStateEnum::JumpLeftOutward;
	if (sType == "JumpRightOutward"s) return PersCurrentStateEnum::JumpRightOutward;
	if (sType == "IdleLeft"s) return PersCurrentStateEnum::IdleLeft;
	if (sType == "IdleRight"s) return PersCurrentStateEnum::IdleRight;
	if (sType == "IdleToward"s) return PersCurrentStateEnum::IdleToward;
	if (sType == "IdleOutward"s) return PersCurrentStateEnum::IdleOutward;
	if (sType == "IdleLeftToward"s) return PersCurrentStateEnum::IdleLeftToward;
	if (sType == "IdleRightToward"s) return PersCurrentStateEnum::IdleRightToward;
	if (sType == "IdleLeftOutward"s) return PersCurrentStateEnum::IdleLeftOutward;
	if (sType == "IdleRightOutward"s) return PersCurrentStateEnum::IdleRightOutward;
	if (sType == "FireLeft"s) return PersCurrentStateEnum::FireLeft;
	if (sType == "FireRight"s) return PersCurrentStateEnum::FireRight;
	if (sType == "FireToward"s) return PersCurrentStateEnum::FireToward;
	if (sType == "FireOutward"s) return PersCurrentStateEnum::FireOutward;
	if (sType == "FireLeftToward"s) return PersCurrentStateEnum::FireLeftToward;
	if (sType == "FireRightToward"s) return PersCurrentStateEnum::FireRightToward;
	if (sType == "FireLeftOutward"s) return PersCurrentStateEnum::FireLeftOutward;
	if (sType == "FireRightOutward"s) return PersCurrentStateEnum::FireRightOutward;
	if (sType == "DieLeft"s) return PersCurrentStateEnum::DieLeft;
	if (sType == "DieRight"s) return PersCurrentStateEnum::DieRight;
	if (sType == "DieToward"s) return PersCurrentStateEnum::DieToward;
	if (sType == "DieOutward"s) return PersCurrentStateEnum::DieOutward;
	if (sType == "DieLeftToward"s) return PersCurrentStateEnum::DieLeftToward;
	if (sType == "DieRightToward"s) return PersCurrentStateEnum::DieRightToward;
	if (sType == "DieLeftOutward"s) return PersCurrentStateEnum::DieLeftOutward;
	if (sType == "DieRightOutward"s) return PersCurrentStateEnum::DieRightOutward;
	return PersCurrentStateEnum::IdleToward;
}

bool TextureAnimStateComponent::VInit(TiXmlElement* pData) {

	TiXmlElement* pAtlasWidth = pData->FirstChildElement("AtlasWidth");
	if (pAtlasWidth) {
		std::string sAtlasWidth = pAtlasWidth->FirstChild()->Value();
		m_atlas_width = std::stoi(sAtlasWidth);
	}

	TiXmlElement* pAtlasHeight = pData->FirstChildElement("AtlasHeight");
	if (pAtlasHeight) {
		std::string sAtlasHeight = pAtlasHeight->FirstChild()->Value();
		m_atlas_height = std::stoi(sAtlasHeight);
	}

	TiXmlElement* pFrame = pData->FirstChildElement("Frame");
	while (pFrame) {
		FrameData frame_data;

		const char* cType = pFrame->Attribute("Type");
		std::string sType(cType == nullptr ? "" : cType);
		PersCurrentStateEnum state_enum = decodeStateEnum(sType);

		const char* cVerticalImageFlip = pFrame->Attribute("VerticalImageFlip");
		std::string sVerticalImageFlip(cVerticalImageFlip == nullptr ? "" : cVerticalImageFlip);
		std::for_each(sVerticalImageFlip.begin(), sVerticalImageFlip.end(), [](char& c) { c = ::toupper(c); });
		frame_data.VerticalFlip = (sVerticalImageFlip == "YES" || sVerticalImageFlip == "TRUE" || sVerticalImageFlip == "1") ? true : false;

		const char* cHorizontalImageFlip = pFrame->Attribute("VerticalImageFlip");
		std::string sHorizontalImageFlip = (cHorizontalImageFlip == nullptr ? "" : cHorizontalImageFlip);
		std::for_each(sHorizontalImageFlip.begin(), sHorizontalImageFlip.end(), [](char& c) { c = ::toupper(c); });
		frame_data.HorizontalFlip = (sHorizontalImageFlip == "YES" || sHorizontalImageFlip == "TRUE" || sHorizontalImageFlip == "1") ? true : false;

		const char* cData = pFrame->Attribute("Data");
		std::string sData(cData == nullptr ? "" : cData);
		auto s_data_vec = splitR<std::vector<std::string>>(sData, ","s);
		std::transform(s_data_vec.cbegin(), s_data_vec.cend(), std::back_inserter(frame_data.Data), [](const std::string& s) {return std::stoi(s);});
		
		const char* cFrameTime = pFrame->Attribute("FrameTime");
		std::string sFrameTime(cFrameTime == nullptr ? "" : cFrameTime);
		frame_data.FrameTime = sFrameTime.empty() ? 0.0f : std::stof(sFrameTime);

		m_frames_data[state_enum] = frame_data;
		pFrame = pFrame->NextSiblingElement("Frame");
	}

	return true;
}

void TextureAnimStateComponent::VPostInit() {}

void TextureAnimStateComponent::VUpdate(float deltaMs) {
	using namespace DirectX;
	m_total_time += deltaMs;
	std::shared_ptr<ParticleComponent> pParticleComponent = MakeStrongPtr(GetOwner()->GetComponent<ParticleComponent>(ParticleComponent::g_Name));
	const FrameData& current_state_data = m_frames_data.at(m_current_state);

	int total_frames_num = current_state_data.Data.size();
	float frame_time = current_state_data.FrameTime;

	float total_anim_time = frame_time * ((float)total_frames_num);
	float loop_time = std::fmodf(m_anim_time, total_anim_time);
	int frame_num_couner = (int)(loop_time / frame_time);
	int current_frame_num = current_state_data.Data[frame_num_couner];
	float frame_shift = current_frame_num % m_atlas_width;
	float row_shift = current_frame_num / m_atlas_width;

	float frame_u = 1.0f / ((float)m_atlas_width);
	float frame_v = 1.0f / ((float)m_atlas_height);

	XMMATRIX scale_to_one_frame = XMMatrixScaling(frame_u, frame_v, 1.0f);
	XMMATRIX translation = XMMatrixTranslation(frame_u * frame_shift, frame_v * row_shift, 0.0f);

	XMMATRIX transform = XMMatrixMultiply(scale_to_one_frame, translation);
	XMStoreFloat4x4(&m_tex_transform, transform);
	m_anim_time += deltaMs;
}

DirectX::XMFLOAT4X4 TextureAnimStateComponent::GetTexTransform4x4() {
	return m_tex_transform;
}

DirectX::XMFLOAT4X4 TextureAnimStateComponent::GetTexTransform4x4T() {
	DirectX::XMFLOAT4X4 res;
	DirectX::XMStoreFloat4x4(&res, DirectX::XMMatrixTranspose(DirectX::XMLoadFloat4x4(&m_tex_transform)));
	return res;
}

PersCurrentStateEnum TextureAnimStateComponent::GetState() {
	return m_current_state;
}

PersCurrentStateClassEnum TextureAnimStateComponent::GetStateClass() {
	PersCurrentStateClassEnum res = PersCurrentStateClassEnum::Idle;
	switch (m_current_state) {
		case PersCurrentStateEnum::WalkLeft:
			res = PersCurrentStateClassEnum::Walk;
			break;
		case PersCurrentStateEnum::WalkRight:
			res = PersCurrentStateClassEnum::Walk;
			break;
		case PersCurrentStateEnum::WalkToward:
			res = PersCurrentStateClassEnum::Walk;
			break;
		case PersCurrentStateEnum::WalkOutward:
			res = PersCurrentStateClassEnum::Walk;
			break;
		case PersCurrentStateEnum::WalkLeftToward:
			res = PersCurrentStateClassEnum::Walk;
			break;
		case PersCurrentStateEnum::WalkRightToward:
			res = PersCurrentStateClassEnum::Walk;
			break;
		case PersCurrentStateEnum::WalkLeftOutward:
			res = PersCurrentStateClassEnum::Walk;
			break;
		case PersCurrentStateEnum::WalkRightOutward:
			res = PersCurrentStateClassEnum::Walk;
			break;
		case PersCurrentStateEnum::JumpLeft:
			res = PersCurrentStateClassEnum::Jump;
			break;
		case PersCurrentStateEnum::JumpRight:
			res = PersCurrentStateClassEnum::Jump;
			break;
		case PersCurrentStateEnum::JumpToward:
			res = PersCurrentStateClassEnum::Jump;
			break;
		case PersCurrentStateEnum::JumpOutward:
			res = PersCurrentStateClassEnum::Jump;
			break;
		case PersCurrentStateEnum::JumpLeftToward:
			res = PersCurrentStateClassEnum::Jump;
			break;
		case PersCurrentStateEnum::JumpRightToward:
			res = PersCurrentStateClassEnum::Jump;
			break;
		case PersCurrentStateEnum::JumpLeftOutward:
			res = PersCurrentStateClassEnum::Jump;
			break;
		case PersCurrentStateEnum::JumpRightOutward:
			res = PersCurrentStateClassEnum::Jump;
			break;
		case PersCurrentStateEnum::IdleLeft:
			res = PersCurrentStateClassEnum::Idle;
			break;
		case PersCurrentStateEnum::IdleRight:
			res = PersCurrentStateClassEnum::Idle;
			break;
		case PersCurrentStateEnum::IdleToward:
			res = PersCurrentStateClassEnum::Idle;
			break;
		case PersCurrentStateEnum::IdleOutward:
			res = PersCurrentStateClassEnum::Idle;
			break;
		case PersCurrentStateEnum::IdleLeftToward:
			res = PersCurrentStateClassEnum::Idle;
			break;
		case PersCurrentStateEnum::IdleRightToward:
			res = PersCurrentStateClassEnum::Idle;
			break;
		case PersCurrentStateEnum::IdleLeftOutward:
			res = PersCurrentStateClassEnum::Idle;
			break;
		case PersCurrentStateEnum::IdleRightOutward:
			res = PersCurrentStateClassEnum::Idle;
			break;
		default:
			break;
	}

	return res;
}

PersCurrentOrientClassEnum TextureAnimStateComponent::GetOrientClass() {
	PersCurrentOrientClassEnum res = PersCurrentOrientClassEnum::Toward;
	switch (m_current_state) {
		case PersCurrentStateEnum::WalkLeft:
			res = PersCurrentOrientClassEnum::Left;
			break;
		case PersCurrentStateEnum::WalkRight:
			res = PersCurrentOrientClassEnum::Right;
			break;
		case PersCurrentStateEnum::WalkToward:
			res = PersCurrentOrientClassEnum::Toward;
			break;
		case PersCurrentStateEnum::WalkOutward:
			res = PersCurrentOrientClassEnum::Outward;
			break;
		case PersCurrentStateEnum::WalkLeftToward:
			res = PersCurrentOrientClassEnum::LeftToward;
			break;
		case PersCurrentStateEnum::WalkRightToward:
			res = PersCurrentOrientClassEnum::RightToward;
			break;
		case PersCurrentStateEnum::WalkLeftOutward:
			res = PersCurrentOrientClassEnum::LeftOutward;
			break;
		case PersCurrentStateEnum::WalkRightOutward:
			res = PersCurrentOrientClassEnum::RightOutward;
			break;
		case PersCurrentStateEnum::JumpLeft:
			res = PersCurrentOrientClassEnum::Left;
			break;
		case PersCurrentStateEnum::JumpRight:
			res = PersCurrentOrientClassEnum::Right;
			break;
		case PersCurrentStateEnum::JumpToward:
			res = PersCurrentOrientClassEnum::Toward;
			break;
		case PersCurrentStateEnum::JumpOutward:
			res = PersCurrentOrientClassEnum::Outward;
			break;
		case PersCurrentStateEnum::JumpLeftToward:
			res = PersCurrentOrientClassEnum::LeftToward;
			break;
		case PersCurrentStateEnum::JumpRightToward:
			res = PersCurrentOrientClassEnum::RightToward;
			break;
		case PersCurrentStateEnum::JumpLeftOutward:
			res = PersCurrentOrientClassEnum::LeftOutward;
			break;
		case PersCurrentStateEnum::JumpRightOutward:
			res = PersCurrentOrientClassEnum::RightOutward;
			break;
		case PersCurrentStateEnum::IdleLeft:
			res = PersCurrentOrientClassEnum::Left;
			break;
		case PersCurrentStateEnum::IdleRight:
			res = PersCurrentOrientClassEnum::Right;
			break;
		case PersCurrentStateEnum::IdleToward:
			res = PersCurrentOrientClassEnum::Toward;
			break;
		case PersCurrentStateEnum::IdleOutward:
			res = PersCurrentOrientClassEnum::Outward;
			break;
		case PersCurrentStateEnum::IdleLeftToward:
			res = PersCurrentOrientClassEnum::LeftToward;
			break;
		case PersCurrentStateEnum::IdleRightToward:
			res = PersCurrentOrientClassEnum::RightToward;
			break;
		case PersCurrentStateEnum::IdleLeftOutward:
			res = PersCurrentOrientClassEnum::LeftOutward;
			break;
		case PersCurrentStateEnum::IdleRightOutward:
			res = PersCurrentOrientClassEnum::RightOutward;
			break;
		default:
			break;
	}

	return res;
}

void TextureAnimStateComponent::SetState(PersCurrentStateEnum state) {
	m_current_state = state;
	m_anim_time = 0.0f;
}

TiXmlElement* TextureAnimStateComponent::VGenerateXml() {
	return nullptr;
}
