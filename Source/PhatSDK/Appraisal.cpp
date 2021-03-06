
#include "StdAfx.h"
#include "Appraisal.h"

DEFINE_PACK(WeaponProfile)
{
	pWriter->Write<int>((int)damage_type);
	pWriter->Write<int>(weapon_time);
	pWriter->Write<unsigned int>(weapon_skill);
	pWriter->Write<int>(weapon_damage);
	pWriter->Write<double>(damage_variance);
	pWriter->Write<double>(damage_mod);
	pWriter->Write<double>(weapon_length);
	pWriter->Write<double>(max_velocity);
	pWriter->Write<double>(weapon_offense);
	pWriter->Write<int>(max_velocity_estimated);
	pWriter->Align();
}

DEFINE_UNPACK(WeaponProfile)
{
	damage_type = (DAMAGE_TYPE)pReader->Read<int>();
	weapon_time = pReader->Read<int>();
	weapon_skill = pReader->Read<unsigned int>();
	weapon_damage = pReader->Read<int>();
	damage_variance = pReader->Read<double>();
	damage_mod = pReader->Read<double>();
	weapon_length = pReader->Read<double>();
	max_velocity = pReader->Read<double>();
	weapon_offense = pReader->Read<double>();
	max_velocity_estimated = pReader->Read<int>();
	pReader->ReadAlign();
	return true;
}

DEFINE_PACK(HookAppraisalProfile)
{
	pWriter->Write<unsigned int>(mBitfield);
	pWriter->Write<unsigned int>(mValidLocations);
	pWriter->Write<unsigned int>(mAmmoType);
}

DEFINE_UNPACK(HookAppraisalProfile)
{
	mBitfield = pReader->Read<unsigned int>();
	mValidLocations = pReader->Read<unsigned int>();
	mAmmoType = pReader->Read<AMMO_TYPE>();
	return true;
}

DEFINE_PACK(ArmorProfile)
{
	pWriter->Write<float>(mod_vs_slash);
	pWriter->Write<float>(mod_vs_pierce);
	pWriter->Write<float>(mod_vs_bludgeon);
	pWriter->Write<float>(mod_vs_cold);
	pWriter->Write<float>(mod_vs_fire);
	pWriter->Write<float>(mod_vs_acid);
	pWriter->Write<float>(mod_vs_nether);
	pWriter->Write<float>(mod_vs_electric);
}

DEFINE_UNPACK(ArmorProfile)
{
	mod_vs_slash = pReader->Read<float>();
	mod_vs_pierce = pReader->Read<float>();
	mod_vs_bludgeon = pReader->Read<float>();
	mod_vs_cold = pReader->Read<float>();
	mod_vs_fire = pReader->Read<float>();
	mod_vs_acid = pReader->Read<float>();
	mod_vs_nether = pReader->Read<float>();
	mod_vs_electric = pReader->Read<float>();
	return true;
}

AppraisalProfile::AppraisalProfile()
{
}

AppraisalProfile::~AppraisalProfile()
{
	SafeDelete(creature_profile);
	SafeDelete(hook_profile);
	SafeDelete(weapon_profile);
	SafeDelete(armor_profile);
	SafeDelete(_intStatsTable);
	SafeDelete(_int64StatsTable);
	SafeDelete(_boolStatsTable);
	SafeDelete(_floatStatsTable);
	SafeDelete(_strStatsTable);
	SafeDelete(_didStatsTable);
	SafeDelete(_spellBook);
}

DEFINE_PACK(AppraisalProfile)
{
	DWORD flags = 0;

	BinaryWriter content;

	if (_intStatsTable)
	{
		flags |= 1;
		_intStatsTable->Pack(&content);
	}
	if (_int64StatsTable)
	{
		flags |= 0x2000;
		_int64StatsTable->Pack(&content);
	}
	if (_boolStatsTable)
	{
		flags |= 2;
		_boolStatsTable->Pack(&content);
	}
	if (_floatStatsTable)
	{
		flags |= 4;
		_floatStatsTable->Pack(&content);
	}
	if (_strStatsTable)
	{
		flags |= 8;
		_strStatsTable->Pack(&content);
	}
	if (_didStatsTable)
	{
		flags |= 0x1000;
		_didStatsTable->Pack(&content);
	}
	if (_spellBook)
	{
		flags |= 0x10;
		_spellBook->Pack(&content);
	}
	if (armor_profile)
	{
		flags |= 0x80;
		armor_profile->Pack(&content);
	}
	if (creature_profile)
	{
		flags |= 0x100;
		creature_profile->Pack(&content);
	}
	if (weapon_profile)
	{
		flags |= 0x20;
		weapon_profile->Pack(&content);
	}
	if (hook_profile)
	{
		flags |= 0x40;
		hook_profile->Pack(&content);
	}
	if (armor_ench_bitfield)
	{
		flags |= 0x200;
		content.Write<DWORD>(armor_ench_bitfield);
	}
	if (weapon_ench_bitfield)
	{
		flags |= 0x800;
		content.Write<DWORD>(weapon_ench_bitfield);
	}
	if (resist_ench_bitfield)
	{
		flags |= 0x400;
		content.Write<DWORD>(resist_ench_bitfield);
	}

	if (base_armor_head
		|| base_armor_chest
		|| base_armor_groin
		|| base_armor_bicep
		|| base_armor_wrist
		|| base_armor_hand
		|| base_armor_thigh
		|| base_armor_shin
		|| base_armor_foot)
	{
		flags |= 0x4000;
		content.Write<DWORD>(base_armor_head);
		content.Write<DWORD>(base_armor_chest);
		content.Write<DWORD>(base_armor_groin);
		content.Write<DWORD>(base_armor_bicep);
		content.Write<DWORD>(base_armor_wrist);
		content.Write<DWORD>(base_armor_hand);
		content.Write<DWORD>(base_armor_thigh);
		content.Write<DWORD>(base_armor_shin);
		content.Write<DWORD>(base_armor_foot);
	}

	pWriter->Write<DWORD>(flags);
	pWriter->Write<int>(success_flag);
	pWriter->Write(&content);
}

DEFINE_UNPACK(AppraisalProfile)
{
	DWORD flags = pReader->Read<DWORD>();
	success_flag = pReader->Read<int>();

	if (flags & 1)
	{
		_intStatsTable = new PackableHashTableWithJson<STypeInt, int>();
		_intStatsTable->UnPack(pReader);
	}
	if (flags & 0x2000)
	{
		_int64StatsTable = new PackableHashTableWithJson<STypeInt64, __int64>();
		_int64StatsTable->UnPack(pReader);
	}
	if (flags & 2)
	{
		_boolStatsTable = new PackableHashTableWithJson<STypeBool, BOOL>();
		_boolStatsTable->UnPack(pReader);
	}
	if (flags & 4)
	{
		_floatStatsTable = new PackableHashTableWithJson<STypeFloat, double>();
		_floatStatsTable->UnPack(pReader);
	}
	if (flags & 8)
	{
		_strStatsTable = new PackableHashTableWithJson<STypeString, std::string>();
		_strStatsTable->UnPack(pReader);
	}
	if (flags & 0x1000)
	{
		_didStatsTable = new PackableHashTableWithJson<STypeDID, DWORD>();
		_didStatsTable->UnPack(pReader);
	}
	if (flags & 0x10)
	{
		_spellBook = new SmartArray<DWORD>();
		_spellBook->UnPack(pReader);
	}
	if (flags & 0x80)
	{
		armor_profile = new ArmorProfile();
		armor_profile->UnPack(pReader);
	}

	if (flags & 0x100)
	{
		creature_profile = new CreatureAppraisalProfile();
		creature_profile->UnPack(pReader);
	}

	if (flags & 0x20)
	{
		weapon_profile = new WeaponProfile();
		weapon_profile->UnPack(pReader);
	}
	if (flags & 0x40)
	{
		hook_profile = new HookAppraisalProfile();
		hook_profile->UnPack(pReader);
	}
	if (flags & 0x200)
	{
		armor_ench_bitfield = pReader->Read<DWORD>();
	}
	if (flags & 0x800)
	{
		weapon_ench_bitfield = pReader->Read<DWORD>();
	}
	if (flags & 0x400)
	{
		resist_ench_bitfield = pReader->Read<DWORD>();
	}

	if (flags & 0x4000)
	{
		base_armor_head = pReader->Read<DWORD>();
		base_armor_chest = pReader->Read<DWORD>();
		base_armor_groin = pReader->Read<DWORD>();
		base_armor_bicep = pReader->Read<DWORD>();
		base_armor_wrist = pReader->Read<DWORD>();
		base_armor_hand = pReader->Read<DWORD>();
		base_armor_thigh = pReader->Read<DWORD>();
		base_armor_shin = pReader->Read<DWORD>();
		base_armor_foot = pReader->Read<DWORD>();
	}

	return true;
}

DEFINE_PACK(CreatureAppraisalProfile)
{
	DWORD flags = 0;
	BinaryWriter content;

	if (max_stamina)
	{
		flags |= 8;
		content.Write<DWORD>(strength);
		content.Write<DWORD>(endurance);
		content.Write<DWORD>(quickness);
		content.Write<DWORD>(coordination);
		content.Write<DWORD>(focus);
		content.Write<DWORD>(self);
		content.Write<DWORD>(stamina);
		content.Write<DWORD>(mana);
		content.Write<DWORD>(max_stamina);
		content.Write<DWORD>(max_mana);
	}

	if (enchantment_bitfield)
	{
		flags |= 1;
		content.Write<DWORD>(enchantment_bitfield);
	}

	pWriter->Write<DWORD>(flags);
	pWriter->Write<int>(health);
	pWriter->Write<int>(max_health);
	pWriter->Write(&content);
}

DEFINE_UNPACK(CreatureAppraisalProfile)
{
	DWORD flags = pReader->Read<DWORD>();
	health = pReader->Read<DWORD>();
	max_health = pReader->Read<DWORD>();

	if (flags & 8)
	{
		strength = pReader->Read<DWORD>();
		endurance = pReader->Read<DWORD>();
		quickness = pReader->Read<DWORD>();
		coordination = pReader->Read<DWORD>();
		focus = pReader->Read<DWORD>();
		self = pReader->Read<DWORD>();
		stamina = pReader->Read<DWORD>();
		mana = pReader->Read<DWORD>();
		max_stamina = pReader->Read<DWORD>();
		max_mana = pReader->Read<DWORD>();
	}
	else
	{
		strength = 0;
		endurance = 0;
		quickness = 0;
		coordination = 0;
		focus = 0;
		self = 0;
		stamina = 0;
		max_stamina = 0;
		mana = 0;
		max_mana = 0;
	}

	if (flags & 1)
	{
		enchantment_bitfield = pReader->Read<DWORD>();
	}
	else
	{
		enchantment_bitfield = 0;
	}

	return true;
}
