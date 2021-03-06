#pragma once

#include <filesystem>
#include <string>

#include <DirectXMath.h>

#include "actor_component.h"
#include "../tools/mt_random.h"

using namespace std::literals;

class CharacterStatsComponent : public ActorComponent {
public:
	static const std::string g_Name;
	virtual const std::string& VGetName() const override;

	CharacterStatsComponent();
	virtual ~CharacterStatsComponent();

	virtual bool VInit(TiXmlElement* pData) override;
	virtual void VPostInit() override;
	virtual void VUpdate(float deltaMs) override;

	int GetCurrentHealth();
	void ApplyHealth(int health);

	int GetTotalHealth();
	void SetHealth(int health);

	bool GetDanger();

	int GetDrops();
	void ApplyDrops(int drops);

	virtual TiXmlElement* VGenerateXml() override;

protected:
	int m_current_health;
	int m_total_health;

	bool m_danger;
	bool m_danger_is_defined;
	float m_danger_radius;

	int m_drops_counter;
};