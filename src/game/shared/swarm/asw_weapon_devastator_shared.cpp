#include "cbase.h"

#include "asw_weapon_devastator_shared.h"
#include "in_buttons.h"
#ifdef CLIENT_DLL
#include "c_asw_player.h"
#include "c_asw_marine.h"
#else
#include "asw_player.h"
#include "asw_marine.h"
#endif

#include "asw_marine_skills.h"
#include "asw_weapon_parse.h"
#include "asw_deathmatch_mode_light.h"
#include "asw_marine_profile.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

IMPLEMENT_NETWORKCLASS_ALIASED(ASW_Weapon_Devastator, DT_ASW_Weapon_Devastator)

BEGIN_NETWORK_TABLE(CASW_Weapon_Devastator, DT_ASW_Weapon_Devastator)
END_NETWORK_TABLE()

BEGIN_PREDICTION_DATA(CASW_Weapon_Devastator)
END_PREDICTION_DATA()

LINK_ENTITY_TO_CLASS(asw_weapon_devastator, CASW_Weapon_Devastator);
PRECACHE_WEAPON_REGISTER(asw_weapon_devastator);

#ifndef CLIENT_DLL
BEGIN_DATADESC(CASW_Weapon_Devastator)
//DEFINE_FIELD( m_bCanShoot, FIELD_TIME ),
END_DATADESC()
#endif

ConVar rd_devastator_dynamic_bullet_spread( "rd_devastator_dynamic_bullet_spread", "1", FCVAR_REPLICATED | FCVAR_CHEAT, "Controls if crouching decreases bullet spread for devastator" );

CASW_Weapon_Devastator::CASW_Weapon_Devastator()
{
}

CASW_Weapon_Devastator::~CASW_Weapon_Devastator()
{

}

void CASW_Weapon_Devastator::SecondaryAttack()
{
	CASW_Player *pPlayer = GetCommander();
	if ( !pPlayer )
		return;

	CASW_Marine *pMarine = GetMarine();
	if ( !pMarine )
		return;

	// dry fire
	SendWeaponAnim( ACT_VM_DRYFIRE );
	BaseClass::WeaponSound( EMPTY );
	m_flNextSecondaryAttack = gpGlobals->curtime + 0.5f;
}

void CASW_Weapon_Devastator::Precache()
{
	PrecacheModel( "swarm/sprites/whiteglow1.vmt" );
	PrecacheModel( "swarm/sprites/greylaser1.vmt");
	PrecacheScriptSound( "ASW_Weapon.Empty" );
	PrecacheScriptSound( "ASW_Weapon_Devastator.SingleFP" );
	PrecacheScriptSound( "ASW_Weapon_Devastator.Single" );
	PrecacheScriptSound( "ASW_Weapon_Devastator.ReloadA" );
	PrecacheScriptSound( "ASW_Weapon_Devastator.ReloadB" );
	PrecacheScriptSound( "ASW_Weapon_Devastator.ReloadC" );

	BaseClass::Precache();
}

float CASW_Weapon_Devastator::GetWeaponBaseDamageOverride()
{
	extern ConVar rd_devastator_dmg_base;
	return rd_devastator_dmg_base.GetFloat();
}
int CASW_Weapon_Devastator::GetWeaponSkillId()
{
	return ASW_MARINE_SKILL_ACCURACY;
}
int CASW_Weapon_Devastator::GetWeaponSubSkillId()
{
	return ASW_MARINE_SUBSKILL_ACCURACY_DEVASTATOR_DMG;
}

float CASW_Weapon_Devastator::GetMovementScale()
{
	return ShouldMarineMoveSlow() ? 0.3f : 0.7f;
}

#ifndef CLIENT_DLL
#else
const char *CASW_Weapon_Devastator::GetPartialReloadSound( int iPart )
{
	switch ( iPart )
	{
	case 1: return "ASW_Weapon_Devastator.ReloadB"; break;
	case 2: return "ASW_Weapon_Devastator.ReloadC"; break;
	default: break;
	};
	return "ASW_Weapon_Devastator.ReloadA";
}
#endif

bool CASW_Weapon_Devastator::ShouldMarineMoveSlow()
{
	bool bAttack1, bAttack2, bReload, bOldReload, bOldAttack1;
	GetButtons(bAttack1, bAttack2, bReload, bOldReload, bOldAttack1);

	return ( BaseClass::ShouldMarineMoveSlow() || bAttack1 );
}

void CASW_Weapon_Devastator::FireShotgunPellet( CASW_Inhabitable_NPC *pNPC, const FireBulletsInfo_t &info, int iSeed )
{
	CASW_Marine *pMarine = CASW_Marine::AsMarine( pNPC );
	if ( !pMarine )
	{
		BaseClass::FireShotgunPellet( pNPC, info, iSeed );
		return;
	}

	float fPiercingChance = MarineSkills()->GetSkillBasedValueByMarine( pMarine, ASW_MARINE_SKILL_STOPPING_POWER, ASW_MARINE_SUBSKILL_PIERCING_CHANCE );
	if (fPiercingChance > 0)
	{
		pMarine->FirePenetratingBullets(info, 1, fPiercingChance, iSeed, false );
	}
	else
	{
		pMarine->FirePenetratingBullets(info, 0, 1.0f, iSeed, false );
	}
}

const Vector &CASW_Weapon_Devastator::GetAngularBulletSpread()
{
	const static Vector cone( 22, 22, 22 );
	const static Vector cone_duck( 14, 14, 14 );

	CASW_Marine *marine = GetMarine();

	if ( marine && rd_devastator_dynamic_bullet_spread.GetBool() )
	{
		if ( marine->GetLocalVelocity().IsZero() && marine->m_bWalking )
			return cone_duck;
	}
	return cone;
}
