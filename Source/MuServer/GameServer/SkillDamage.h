// SkillDamage.h: interface for the CSkillDamage class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "Skill.h"

struct SKILL_DAMAGE_INFO
{
	int Index;
	int RatePvP;
	int RatePvM;
};

class CSkillDamage
{
public:

	CSkillDamage();

	virtual ~CSkillDamage();

	void Load(char* path);

	void SetInfo(int type, SKILL_DAMAGE_INFO info);

	int GetDamageRate(CSkill* lpSkill, int damage, bool isPvP);

	int GetEffectRate(CSkill* lpSkill, bool isPvP);

private:

	std::map<int, SKILL_DAMAGE_INFO> m_SkillDamageRateInfo;
	std::map<int, SKILL_DAMAGE_INFO> m_SkillEffectRateInfo;
};

extern CSkillDamage gSkillDamage;