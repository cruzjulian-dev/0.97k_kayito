#include "stdafx.h"
#include "SkillDamage.h"
#include "ReadScript.h"
#include "Util.h"

CSkillDamage gSkillDamage;

CSkillDamage::CSkillDamage()
{
	this->m_SkillDamageRateInfo.clear();

	this->m_SkillEffectRateInfo.clear();
}

CSkillDamage::~CSkillDamage()
{
}

void CSkillDamage::Load(char* path)
{
	CReadScript* lpReadScript = new CReadScript;

	if (lpReadScript == NULL)
	{
		ErrorMessageBox(READ_SCRIPT_ALLOC_ERROR, path);

		return;
	}

	if (!lpReadScript->Load(path))
	{
		ErrorMessageBox(READ_SCRIPT_FILE_ERROR, path);

		delete lpReadScript;

		return;
	}

	this->m_SkillDamageRateInfo.clear();
	this->m_SkillEffectRateInfo.clear();

	try
	{
		eTokenResult token;

		while (true)
		{
			token = lpReadScript->GetToken();

			if (token == TOKEN_END || token == TOKEN_END_SECTION)
			{
				break;
			}

			int section = lpReadScript->GetNumber();

			while (true)
			{
				token = lpReadScript->GetToken();

				if (token == TOKEN_END || token == TOKEN_END_SECTION)
				{
					break;
				}

				if (section == 0)
				{
					SKILL_DAMAGE_INFO info;

					info.Index = lpReadScript->GetNumber();

					info.RatePvP = lpReadScript->GetAsNumber();

					info.RatePvM= lpReadScript->GetAsNumber();

					this->SetInfo(0, info);
				}
				else if (section == 1)
				{
					SKILL_DAMAGE_INFO info;

					info.Index = lpReadScript->GetNumber();

					info.RatePvP = lpReadScript->GetAsNumber();

					info.RatePvM = lpReadScript->GetAsNumber();

					this->SetInfo(1, info);
				}
				else 
				{
					break;
				}
			}
		}
	}
	catch (...)
	{
		ErrorMessageBox(lpReadScript->GetError());
	}

	delete lpReadScript;
}


void CSkillDamage::SetInfo(int type, SKILL_DAMAGE_INFO info)
{
	if (type == 0)
	{
		if (this->m_SkillDamageRateInfo.find(info.Index) == this->m_SkillDamageRateInfo.end())
		{
			this->m_SkillDamageRateInfo.insert(std::pair<int, SKILL_DAMAGE_INFO>(info.Index, info));
		}
	}
	else if (type == 1)
	{
		if (this->m_SkillEffectRateInfo.find(info.Index) == this->m_SkillEffectRateInfo.end())
		{
			this->m_SkillEffectRateInfo.insert(std::pair<int, SKILL_DAMAGE_INFO>(info.Index, info));
		}
	}
}

int CSkillDamage::GetDamageRate(CSkill* lpSkill, int damage, int type)
{
	if (lpSkill != 0)
	{
		std::map<int, SKILL_DAMAGE_INFO>::iterator it = this->m_SkillDamageRateInfo.find(lpSkill->m_index);

		if (it != this->m_SkillDamageRateInfo.end())
		{
			int rate = 100;
			
			if (type == OBJECT_USER)
			{
				rate = it->second.RatePvP;

			} else {

				rate = it->second.RatePvM;
			} 

			damage = (damage * rate) / 100;
		}
	}

	return damage;
}

bool CSkillDamage::GetEffectRate(int skill, int type) // OK
{
	if (skill != 0)
	{
		std::map<int, SKILL_DAMAGE_INFO>::iterator it = this->m_SkillEffectRateInfo.find(skill);

		if (it != this->m_SkillEffectRateInfo.end())
		{
			int rate = 100;

			if (type == OBJECT_USER)
			{
				rate = it->second.RatePvP;

			}
			else {

				rate = it->second.RatePvM;
			}

			if ((GetLargeRand() % 100) < rate)
			{
				return 1;
			}
		}
	}

	return 0;
}