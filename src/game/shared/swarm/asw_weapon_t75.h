#ifndef _INCLUDED_ASW_WEAPON_T75_H
#define _INCLUDED_ASW_WEAPON_T75_H
#pragma once

#ifdef CLIENT_DLL
#include "c_asw_weapon.h"
#define CASW_Weapon C_ASW_Weapon
#define CASW_Weapon_T75 C_ASW_Weapon_T75
#define CASW_Marine C_ASW_Marine
#else
#include "asw_weapon.h"
#include "npc_combine.h"
#endif

#include "basegrenade_shared.h"
#include "asw_shareddefs.h"

class CASW_Weapon_T75 : public CASW_Weapon
{
public:
	DECLARE_CLASS( CASW_Weapon_T75, CASW_Weapon );
	DECLARE_NETWORKCLASS();
	DECLARE_PREDICTABLE();

	CASW_Weapon_T75();
	virtual ~CASW_Weapon_T75();
	void Precache();
	float	GetFireRate( void ) { return 1.4f; }
	bool	Reload();
	void	ItemPostFrame();
	virtual bool ShouldMarineMoveSlow() { return false; }	// firing doesn't slow the marine down
	
	Activity	GetPrimaryAttackActivity( void );

	void	PrimaryAttack();
	bool	OffhandActivate();

	virtual const Vector& GetBulletSpread( void )
	{
		static const Vector cone = VECTOR_CONE_PRECALCULATED;

		return cone;
	}

	#ifndef CLIENT_DLL
		DECLARE_DATADESC();

		int		CapabilitiesGet( void ) { return bits_CAP_WEAPON_RANGE_ATTACK1; }
	#else
	
	#endif

	virtual bool IsOffensiveWeapon() { return false; }

	float	m_flSoonestPrimaryAttack;

	// Classification
	virtual Class_T		Classify( void ) { return (Class_T) CLASS_ASW_T75; }
};


#endif /* _INCLUDED_ASW_WEAPON_T75_H */
