
#include "StdAfx.h"
#include "PhatSDK.h"
#include "SkillChecks.h"

double GetSkillChance(int skill, int difficulty)
{
	double chance = 1.0 - (1.0 / (1.0 + exp(0.03 * (skill - difficulty))));

	return min(1.0, max(0.0, chance));
}

double GetAppraisalSkillChance(int skill, int difficulty)
{
	double chance = 1.0 - (1.0 / (1.0 + exp(0.05 * (skill - difficulty))));

	return min(1.0, max(0.0, chance));
}

double GetMagicSkillChance(int skill, int difficulty)
{
	double chance = 1.0 - (1.0 / (1.0 + exp(0.07 * (skill - difficulty))));

	return min(1.0, max(0.0, chance));
}

int GetManaCost(int skill, int difficulty, int manaCost, int manaConversion)
{
	if (!manaConversion)
		return manaCost;

	double conversionFactor = min(1.0, (double)difficulty / (double)manaConversion);

	int baseManaCost = round(conversionFactor * manaCost);

	if (conversionFactor < 1.0)
	{
	conversionFactor = Random::RollDice(0.0, conversionFactor);

	if (conversionFactor == 0.0)
	{
		return 1;
	}
	}
	return (int)(manaCost * conversionFactor);
}

bool GenericSkillCheck(int offense, int defense)
{
	double chance = GetSkillChance(offense, defense);

	if (Random::RollDice(0.0, 1.0) <= chance)
	{
		// succeeded
		return true;
	}

	// failed
	return false;
}

bool AppraisalSkillCheck(int offense, int defense)
{
	double chance = GetAppraisalSkillChance(offense, defense);

	if (Random::RollDice(0.0, 1.0) <= chance)
	{
		// succeeded
		return true;
	}

	// failed
	return false;
}

bool TryMagicResist(int offense, int defense)
{
	return !GenericSkillCheck(offense, defense);
}

bool TryMeleeEvade(int offense, int defense)
{
	return !GenericSkillCheck(offense, defense);
}

bool TryMissileEvade(int offense, int defense)
{
	return !GenericSkillCheck(offense, defense);
}
