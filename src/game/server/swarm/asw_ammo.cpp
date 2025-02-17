#include "cbase.h"
#include "asw_ammo.h"
#include "gamerules.h"
#include "items.h"
#include "ammodef.h"
#include "asw_player.h"
#include "asw_marine.h"
#include "asw_ammo_drop_shared.h"
#include "particle_parse.h"
#include "cvisibilitymonitor.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"


extern ConVar asw_ammo_count_ar2;
extern ConVar asw_ammo_count_autogun;
extern ConVar asw_ammo_count_cryo_cannon;
extern ConVar asw_ammo_count_devastator;
extern ConVar asw_ammo_count_flamer;
extern ConVar asw_ammo_count_grenade_launcher;
extern ConVar asw_ammo_count_heavy_rifle;
extern ConVar asw_ammo_count_internal_chainsaw;
extern ConVar asw_ammo_count_mining_laser;
extern ConVar asw_ammo_count_pdw;
extern ConVar asw_ammo_count_pistol;
extern ConVar asw_ammo_count_plasma_thrower;
extern ConVar asw_ammo_count_railgun;
extern ConVar asw_ammo_count_rifle;
extern ConVar asw_ammo_count_rifle_burst;
extern ConVar asw_ammo_count_shotgun;
extern ConVar asw_ammo_count_sniper_rifle;
extern ConVar asw_ammo_count_vindicator;

//-------------
// Generic give ammo function
//-------------

int ASW_GiveAmmo( CASW_Inhabitable_NPC *pNPC, float flCount, const char *pszAmmoName, CBaseEntity *pAmmoEntity, bool bSuppressSound = false )
{
	if ( pAmmoEntity->IsMarkedForDeletion() )
	{
		return 0;
	}

	int iAmmoType = GetAmmoDef()->Index( pszAmmoName );
	if ( iAmmoType == -1 )
	{
		Msg( "ERROR: Attempting to give unknown ammo type (%s)\n", pszAmmoName );
		return 0;
	}

	CASW_Marine *pMarine = CASW_Marine::AsMarine( pNPC );
	if ( !pMarine )
	{
		return 0;
	}

	int amount = pMarine->GiveAmmo( flCount, iAmmoType, bSuppressSound );
	if ( amount == 0 )
		amount = pMarine->GiveAmmoToAmmoBag( flCount, iAmmoType, bSuppressSound );

	if ( amount > 0 )
	{
		pMarine->TookAmmoPickup( pAmmoEntity );

		// Check the ammo type... for some doing a spilling bullet effect isn't fictionally appropriate
		// (disabled because it's better to give *some* feedback and there are still bullets in there even if we're not pulling them out for this gun)
		//if ( iAmmoType != GetAmmoDef()->Index( "ASW_F" ) && iAmmoType != GetAmmoDef()->Index( "ASW_ML" ) && iAmmoType != GetAmmoDef()->Index( "ASW_TG" ) && iAmmoType != GetAmmoDef()->Index( "ASW_GL" ) )
		{
			// Do effects
			int iAmmoCost = CASW_Ammo_Drop_Shared::GetAmmoUnitCost( iAmmoType );

			if ( iAmmoCost < 20 )
			{
				pAmmoEntity->EmitSound( "ASW_Ammobag.Pickup_sml" );
				DispatchParticleEffect( "ammo_satchel_take_sml", pAmmoEntity->GetAbsOrigin() + Vector( 0, 0, 4 ), vec3_angle );
			}
			else if ( iAmmoCost < 75 )
			{
				pAmmoEntity->EmitSound( "ASW_Ammobag.Pickup_med" );
				DispatchParticleEffect( "ammo_satchel_take_med", pAmmoEntity->GetAbsOrigin() + Vector( 0, 0, 4 ), vec3_angle );
			}
			else
			{
				pAmmoEntity->EmitSound( "ASW_Ammobag.Pickup_lrg" );
				DispatchParticleEffect( "ammo_satchel_take_lrg", pAmmoEntity->GetAbsOrigin() + Vector( 0, 0, 4 ), vec3_angle );
			}
		}

		IGameEvent *event = gameeventmanager->CreateEvent( "ammo_pickup" );
		if ( event )
		{
			CASW_Player *pPlayer = pMarine->GetCommander();
			event->SetInt( "userid", ( pPlayer ? pPlayer->GetUserID() : 0 ) );
			event->SetInt( "entindex", pMarine->entindex() );

			gameeventmanager->FireEvent( event );
		}
	}

	return amount;
}

void ASW_GiveSecondaryAmmo( CASW_Inhabitable_NPC *pNPC, std::initializer_list<int> allowedClasses )
{
	CBaseCombatWeapon *pActive = pNPC->GetActiveWeapon();
	if ( pActive && std::find( allowedClasses.begin(), allowedClasses.end(), pActive->Classify() ) != allowedClasses.end() )
	{
		if ( pActive->Clip2() < pActive->GetMaxClip2() )
		{
			pActive->m_iClip2++;
			return;
		}
	}

	CBaseCombatWeapon *pWeapon;
	for ( int i = 0; i < ASW_MAX_MARINE_WEAPONS; i++ )
	{
		pWeapon = pNPC->GetWeapon( i );
		if ( pWeapon == pActive )
			continue;

		if ( pWeapon && std::find( allowedClasses.begin(), allowedClasses.end(), pWeapon->Classify() ) != allowedClasses.end() )
		{
			if ( pWeapon->Clip2() < pWeapon->GetMaxClip2() )
			{
				pWeapon->m_iClip2++;
				return;
			}
		}
	}
}

IMPLEMENT_AUTO_LIST( IAmmoPickupAutoList );

void CASW_Ammo::Spawn( void )
{
	BaseClass::Spawn();

	VisibilityMonitor_AddEntity( this, asw_visrange_generic.GetFloat() * 0.9f, &CASW_Ammo::VismonCallback, NULL );
}


//---------------------------------------------------------
// Callback for the visibility monitor.
//---------------------------------------------------------
bool CASW_Ammo::VismonCallback( CBaseEntity *pPickup, CBasePlayer *pViewingPlayer )
{
	CASW_Ammo *pPickupPtr = dynamic_cast < CASW_Ammo * >( pPickup );

	if ( !pPickupPtr )
		return true;

	IGameEvent *event = gameeventmanager->CreateEvent( "entity_visible" );
	if ( event )
	{
		event->SetInt( "userid", pViewingPlayer->GetUserID() );
		event->SetInt( "subject", pPickupPtr->entindex() );
		event->SetString( "classname", pPickupPtr->GetClassname() );
		event->SetString( "entityname", STRING( pPickupPtr->GetEntityName() ) );
		gameeventmanager->FireEvent( event );
	}

	return false;
}


//-------------
// Rifle Ammo
//-------------

IMPLEMENT_SERVERCLASS_ST( CASW_Ammo_Rifle, DT_ASW_Ammo_Rifle )
END_SEND_TABLE()

BEGIN_DATADESC( CASW_Ammo_Rifle )
DEFINE_KEYFIELD( m_bAddSecondary, FIELD_BOOLEAN, "AddSecondary" ),
END_DATADESC()

LINK_ENTITY_TO_CLASS( asw_ammo_rifle, CASW_Ammo_Rifle );

void CASW_Ammo_Rifle::Spawn( void )
{
	Precache();
	SetModel( "models/swarm/ammo/ammoassaultrifle.mdl" );
	BaseClass::Spawn();
	m_iAmmoIndex = GetAmmoDef()->Index( "ASW_R" );
	m_iAmmoIndex2 = GetAmmoDef()->Index( "ASW_R_BURST" );
}


void CASW_Ammo_Rifle::Precache( void )
{
	PrecacheModel( "models/swarm/ammo/ammoassaultrifle.mdl" );
}

void CASW_Ammo_Rifle::ActivateUseIcon( CASW_Inhabitable_NPC *pNPC, int nHoldType )
{
	if ( nHoldType == ASW_USE_HOLD_START )
		return;

	// if we have a burst rifle active, try that first
	bool bBurstRifleActive = pNPC->GetActiveWeapon() && pNPC->GetActiveWeapon()->GetPrimaryAmmoType() == GetAmmoDef()->Index( "ASW_R_BURST" );
	if ( bBurstRifleActive && ASW_GiveAmmo( pNPC, asw_ammo_count_rifle_burst.GetInt(), "ASW_R_BURST", this ) )
	{
		if ( m_bAddSecondary ) // add rifle grenade
		{
			ASW_GiveSecondaryAmmo( pNPC, { CLASS_ASW_ENERGY_SHIELD } );
		}

		UTIL_Remove( this );
		return;
	}

	// player has used this item
	if ( ASW_GiveAmmo( pNPC, asw_ammo_count_rifle.GetInt(), "ASW_R", this ) )
	{
		if ( m_bAddSecondary ) // add rifle grenade
		{
			ASW_GiveSecondaryAmmo( pNPC, { CLASS_ASW_RIFLE, CLASS_ASW_PRIFLE, CLASS_ASW_COMBAT_RIFLE } );
		}

		UTIL_Remove( this );
		return;
	}

	// alternate use: burst rifle
	if ( !bBurstRifleActive && ASW_GiveAmmo( pNPC, asw_ammo_count_rifle_burst.GetInt(), "ASW_R_BURST", this ) )
	{
		if ( m_bAddSecondary ) // add rifle grenade
		{
			ASW_GiveSecondaryAmmo( pNPC, { CLASS_ASW_ENERGY_SHIELD } );
		}

		UTIL_Remove( this );
		return;
	}
}

//-------------
// Autogun Ammo
//-------------

IMPLEMENT_SERVERCLASS_ST( CASW_Ammo_Autogun, DT_ASW_Ammo_Autogun )
END_SEND_TABLE()

BEGIN_DATADESC( CASW_Ammo_Autogun )
END_DATADESC()

LINK_ENTITY_TO_CLASS( asw_ammo_autogun, CASW_Ammo_Autogun );

void CASW_Ammo_Autogun::Spawn( void )
{
	Precache();
	SetModel( "models/swarm/ammo/ammoautogun.mdl" );
	BaseClass::Spawn();
	m_iAmmoIndex = GetAmmoDef()->Index( "ASW_AG" );
	m_iAmmoIndex2 = GetAmmoDef()->Index( "ASW_DEVASTATOR" );
}


void CASW_Ammo_Autogun::Precache( void )
{
	PrecacheModel( "models/swarm/ammo/ammoautogun.mdl" );
}

void CASW_Ammo_Autogun::ActivateUseIcon( CASW_Inhabitable_NPC *pNPC, int nHoldType )
{
	if ( nHoldType == ASW_USE_HOLD_START )
		return;

	bool bDevastatorActive = pNPC->GetActiveWeapon() && pNPC->GetActiveWeapon()->GetPrimaryAmmoType() == GetAmmoDef()->Index( "ASW_DEVASTATOR" );
	if ( bDevastatorActive && ASW_GiveAmmo( pNPC, asw_ammo_count_devastator.GetInt(), "ASW_DEVASTATOR", this ) )
	{
		UTIL_Remove( this );
		return;
	}

	// player has used this item
	if ( ASW_GiveAmmo( pNPC, asw_ammo_count_autogun.GetInt(), "ASW_AG", this ) )
	{
		UTIL_Remove( this );
		return;
	}

	if ( !bDevastatorActive && ASW_GiveAmmo( pNPC, asw_ammo_count_devastator.GetInt(), "ASW_DEVASTATOR", this ) )
	{
		UTIL_Remove( this );
		return;
	}
}

//-------------
// Shotgun Ammo
//-------------

IMPLEMENT_SERVERCLASS_ST( CASW_Ammo_Shotgun, DT_ASW_Ammo_Shotgun )
END_SEND_TABLE()

BEGIN_DATADESC( CASW_Ammo_Shotgun )
END_DATADESC()

LINK_ENTITY_TO_CLASS( asw_ammo_shotgun, CASW_Ammo_Shotgun );

void CASW_Ammo_Shotgun::Spawn( void )
{
	Precache();
	SetModel( "models/swarm/Ammo/ammoshotgun.mdl" );
	BaseClass::Spawn();
	m_iAmmoIndex = GetAmmoDef()->Index( "ASW_SG" );
}


void CASW_Ammo_Shotgun::Precache( void )
{
	PrecacheModel( "models/swarm/Ammo/ammoshotgun.mdl" );
}

void CASW_Ammo_Shotgun::ActivateUseIcon( CASW_Inhabitable_NPC *pNPC, int nHoldType )
{
	if ( nHoldType == ASW_USE_HOLD_START )
		return;

	if ( ASW_GiveAmmo( pNPC, asw_ammo_count_shotgun.GetInt() * 2, "ASW_SG", this ) )		// two clips per pack
	{
		UTIL_Remove( this );
	}
}

//-------------
// Assault Shotgun Ammo
//-------------

IMPLEMENT_SERVERCLASS_ST( CASW_Ammo_Assault_Shotgun, DT_ASW_Ammo_Assault_Shotgun )
END_SEND_TABLE()

BEGIN_DATADESC( CASW_Ammo_Assault_Shotgun )
DEFINE_KEYFIELD( m_bAddSecondary, FIELD_BOOLEAN, "AddSecondary" ),
END_DATADESC()

LINK_ENTITY_TO_CLASS( asw_ammo_vindicator, CASW_Ammo_Assault_Shotgun );

void CASW_Ammo_Assault_Shotgun::Spawn( void )
{
	Precache();
	SetModel( "models/swarm/Ammo/ammovindicator.mdl" );
	BaseClass::Spawn();
	m_iAmmoIndex = GetAmmoDef()->Index( "ASW_ASG" );
}


void CASW_Ammo_Assault_Shotgun::Precache( void )
{
	PrecacheModel( "models/swarm/Ammo/ammovindicator.mdl" );
}

void CASW_Ammo_Assault_Shotgun::ActivateUseIcon( CASW_Inhabitable_NPC *pNPC, int nHoldType )
{
	if ( nHoldType == ASW_USE_HOLD_START )
		return;

	if ( ASW_GiveAmmo( pNPC, asw_ammo_count_vindicator.GetInt(), "ASW_ASG", this ) )
	{
		if ( m_bAddSecondary )
		{
			ASW_GiveSecondaryAmmo( pNPC, { CLASS_ASW_ASSAULT_SHOTGUN } );
		}

		UTIL_Remove( this );
		return;
	}
}

//-------------
// Flamer Ammo
//-------------

IMPLEMENT_SERVERCLASS_ST( CASW_Ammo_Flamer, DT_ASW_Ammo_Flamer )
END_SEND_TABLE()

BEGIN_DATADESC( CASW_Ammo_Flamer )
END_DATADESC()

LINK_ENTITY_TO_CLASS( asw_ammo_flamer, CASW_Ammo_Flamer );

void CASW_Ammo_Flamer::Spawn( void )
{
	Precache();
	SetModel( "models/swarm/Ammo/ammoflamer.mdl" );
	BaseClass::Spawn();
	m_iAmmoIndex = GetAmmoDef()->Index( "ASW_F" );
	m_iAmmoIndex2 = GetAmmoDef()->Index( "ASW_CRYO" );
	m_iAmmoIndex3 = GetAmmoDef()->Index( "ASW_PLASMA" );
}


void CASW_Ammo_Flamer::Precache( void )
{
	PrecacheModel( "models/swarm/Ammo/ammoflamer.mdl" );
}

void CASW_Ammo_Flamer::ActivateUseIcon( CASW_Inhabitable_NPC *pNPC, int nHoldType )
{
	if ( nHoldType == ASW_USE_HOLD_START )
		return;

	bool bCryoActive = pNPC->GetActiveWeapon() && pNPC->GetActiveWeapon()->GetPrimaryAmmoType() == GetAmmoDef()->Index( "ASW_CRYO" );
	if ( bCryoActive && ASW_GiveAmmo( pNPC, asw_ammo_count_cryo_cannon.GetInt(), "ASW_CRYO", this ) )
	{
		UTIL_Remove( this );
		return;
	}

	bool bPlasmaActive = pNPC->GetActiveWeapon() && pNPC->GetActiveWeapon()->GetPrimaryAmmoType() == GetAmmoDef()->Index( "ASW_PLASMA" );
	if ( bPlasmaActive && ASW_GiveAmmo( pNPC, asw_ammo_count_plasma_thrower.GetInt(), "ASW_PLASMA", this ) )
	{
		UTIL_Remove( this );
		return;
	}

	if ( ASW_GiveAmmo( pNPC, asw_ammo_count_flamer.GetInt(), "ASW_F", this ) )
	{
		UTIL_Remove( this );
		return;
	}

	if ( !bCryoActive && ASW_GiveAmmo( pNPC, asw_ammo_count_cryo_cannon.GetInt(), "ASW_CRYO", this ) )
	{
		UTIL_Remove( this );
		return;
	}

	if ( !bPlasmaActive && ASW_GiveAmmo( pNPC, asw_ammo_count_plasma_thrower.GetInt(), "ASW_PLASMA", this ) )
	{
		UTIL_Remove( this );
		return;
	}
}

//-------------
// Pistol Ammo
//-------------

IMPLEMENT_SERVERCLASS_ST( CASW_Ammo_Pistol, DT_ASW_Ammo_Pistol )
END_SEND_TABLE()

BEGIN_DATADESC( CASW_Ammo_Pistol )
END_DATADESC()

LINK_ENTITY_TO_CLASS( asw_ammo_pistol, CASW_Ammo_Pistol );

void CASW_Ammo_Pistol::Spawn( void )
{
	Precache();
	SetModel( "models/swarm/Ammo/ammopistol.mdl" );
	BaseClass::Spawn();
	m_iAmmoIndex = GetAmmoDef()->Index( "ASW_P" );
}


void CASW_Ammo_Pistol::Precache( void )
{
	PrecacheModel( "models/swarm/Ammo/ammopistol.mdl" );
}

void CASW_Ammo_Pistol::ActivateUseIcon( CASW_Inhabitable_NPC *pNPC, int nHoldType )
{
	if ( nHoldType == ASW_USE_HOLD_START )
		return;

	if ( ASW_GiveAmmo( pNPC, asw_ammo_count_pistol.GetInt(), "ASW_P", this ) )
	{
		UTIL_Remove( this );
		return;
	}
}

//-------------
// Mining Laser Battery Ammo
//-------------

IMPLEMENT_SERVERCLASS_ST( CASW_Ammo_Mining_Laser, DT_ASW_Ammo_Mining_Laser )
END_SEND_TABLE()

BEGIN_DATADESC( CASW_Ammo_Mining_Laser )
END_DATADESC()

LINK_ENTITY_TO_CLASS( asw_ammo_mining_laser, CASW_Ammo_Mining_Laser );

void CASW_Ammo_Mining_Laser::Spawn( void )
{
	Precache();
	SetModel( "models/swarm/Ammo/ammobattery.mdl" );
	BaseClass::Spawn();
	m_iAmmoIndex = GetAmmoDef()->Index( "ASW_ML" );
}


void CASW_Ammo_Mining_Laser::Precache( void )
{
	PrecacheModel( "models/swarm/Ammo/ammobattery.mdl" );
}

void CASW_Ammo_Mining_Laser::ActivateUseIcon( CASW_Inhabitable_NPC *pNPC, int nHoldType )
{
	if ( nHoldType == ASW_USE_HOLD_START )
		return;

	if ( ASW_GiveAmmo( pNPC, asw_ammo_count_mining_laser.GetInt(), "ASW_ML", this) )
	{
		UTIL_Remove( this );
		return;
	}
}

//-------------
// Railgun Ammo
//-------------

IMPLEMENT_SERVERCLASS_ST( CASW_Ammo_Railgun, DT_ASW_Ammo_Railgun )
END_SEND_TABLE()

BEGIN_DATADESC( CASW_Ammo_Railgun )
END_DATADESC()

//LINK_ENTITY_TO_CLASS(asw_ammo_railgun, CASW_Ammo_Railgun);

void CASW_Ammo_Railgun::Spawn( void )
{
	Precache();
	SetModel( "models/swarm/Ammo/ammorailgun.mdl" );
	BaseClass::Spawn();
	m_iAmmoIndex = GetAmmoDef()->Index( "ASW_RG" );
}


void CASW_Ammo_Railgun::Precache( void )
{
	PrecacheModel( "models/swarm/Ammo/ammorailgun.mdl" );
}

void CASW_Ammo_Railgun::ActivateUseIcon( CASW_Inhabitable_NPC *pNPC, int nHoldType )
{
	if ( nHoldType == ASW_USE_HOLD_START )
		return;

	if ( ASW_GiveAmmo( pNPC, asw_ammo_count_railgun.GetInt() * 12, "ASW_RG", this ) )
	{
		UTIL_Remove( this );
		return;
	}
}

//-------------
// Chainsaw Ammo
//-------------

IMPLEMENT_SERVERCLASS_ST( CASW_Ammo_Chainsaw, DT_ASW_Ammo_Chainsaw )
END_SEND_TABLE()

BEGIN_DATADESC( CASW_Ammo_Chainsaw )
END_DATADESC()

LINK_ENTITY_TO_CLASS( asw_ammo_chainsaw, CASW_Ammo_Chainsaw );

void CASW_Ammo_Chainsaw::Spawn( void )
{
	Precache();
	SetModel( "models/swarm/Ammo/ammobattery.mdl" );
	BaseClass::Spawn();
	m_iAmmoIndex = GetAmmoDef()->Index( "ASW_CS" );
}


void CASW_Ammo_Chainsaw::Precache( void )
{
	PrecacheModel( "models/swarm/Ammo/ammobattery.mdl" );
}

void CASW_Ammo_Chainsaw::ActivateUseIcon( CASW_Inhabitable_NPC *pNPC, int nHoldType )
{
	if ( nHoldType == ASW_USE_HOLD_START )
		return;

	if ( ASW_GiveAmmo( pNPC, asw_ammo_count_internal_chainsaw.GetInt(), "ASW_CS", this) )
	{
		UTIL_Remove( this );
		return;
	}
}

//-------------
// PDW Ammo
//-------------

IMPLEMENT_SERVERCLASS_ST( CASW_Ammo_PDW, DT_ASW_Ammo_PDW )
END_SEND_TABLE()

BEGIN_DATADESC( CASW_Ammo_PDW )
END_DATADESC()

LINK_ENTITY_TO_CLASS( asw_ammo_pdw, CASW_Ammo_PDW );

void CASW_Ammo_PDW::Spawn( void )
{
	Precache();
	SetModel( "models/swarm/Ammo/ammopdw.mdl" );
	BaseClass::Spawn();
	m_iAmmoIndex = GetAmmoDef()->Index( "ASW_PDW" );
}


void CASW_Ammo_PDW::Precache( void )
{
	PrecacheModel( "models/swarm/Ammo/ammopdw.mdl" );
}

void CASW_Ammo_PDW::ActivateUseIcon( CASW_Inhabitable_NPC *pNPC, int nHoldType )
{
	if ( nHoldType == ASW_USE_HOLD_START )
		return;

	if ( ASW_GiveAmmo( pNPC, asw_ammo_count_pdw.GetInt(), "ASW_PDW", this) )
	{
		UTIL_Remove( this );
		return;
	}
}

//-------------
// AR2 Ammo
//-------------

IMPLEMENT_SERVERCLASS_ST( CASW_Ammo_AR2, DT_ASW_Ammo_AR2 )
END_SEND_TABLE()

BEGIN_DATADESC( CASW_Ammo_AR2 )
DEFINE_KEYFIELD( m_bAddSecondary, FIELD_BOOLEAN, "AddSecondary" ),
END_DATADESC()

LINK_ENTITY_TO_CLASS( asw_ammo_ar2, CASW_Ammo_AR2 );

void CASW_Ammo_AR2::Spawn( void )
{
	Precache();
	SetModel( "models/swarm/ammo/ammoar2.mdl" );
	BaseClass::Spawn();
	m_iAmmoIndex = GetAmmoDef()->Index( "AR2" );
}


void CASW_Ammo_AR2::Precache( void )
{
	PrecacheModel( "models/swarm/ammo/ammoar2.mdl" );
}

void CASW_Ammo_AR2::ActivateUseIcon( CASW_Inhabitable_NPC *pNPC, int nHoldType )
{
	if ( nHoldType == ASW_USE_HOLD_START )
		return;

	// give two reloads rather than just one like other ammo types
	if ( ASW_GiveAmmo( pNPC, asw_ammo_count_ar2.GetInt() * 2, "AR2", this ) )
	{
		if ( m_bAddSecondary )
		{
			ASW_GiveSecondaryAmmo( pNPC, { CLASS_ASW_AR2 } );
		}

		UTIL_Remove( this );
		return;
	}
}

//-------------
// Grenade Launcher Ammo
//-------------

IMPLEMENT_SERVERCLASS_ST( CASW_Ammo_Grenade_Launcher, DT_ASW_Ammo_Grenade_Launcher )
END_SEND_TABLE()

BEGIN_DATADESC( CASW_Ammo_Grenade_Launcher )
END_DATADESC()

LINK_ENTITY_TO_CLASS( asw_ammo_grenade_launcher, CASW_Ammo_Grenade_Launcher );

void CASW_Ammo_Grenade_Launcher::Spawn( void )
{
	Precache();
	SetModel( "models/swarm/ammo/ammogrenadelauncher.mdl" );
	BaseClass::Spawn();
	m_iAmmoIndex = GetAmmoDef()->Index( "ASW_GL" );
}


void CASW_Ammo_Grenade_Launcher::Precache( void )
{
	PrecacheModel( "models/swarm/ammo/ammogrenadelauncher.mdl" );
}

void CASW_Ammo_Grenade_Launcher::ActivateUseIcon( CASW_Inhabitable_NPC *pNPC, int nHoldType )
{
	if ( nHoldType == ASW_USE_HOLD_START )
		return;

	// give three reloads to match the ammo stash interaction
	if ( ASW_GiveAmmo( pNPC, asw_ammo_count_grenade_launcher.GetInt() * 3, "ASW_GL", this ) )
	{
		UTIL_Remove( this );
		return;
	}
}

//-------------
// Sniper Rifle Ammo
//-------------

IMPLEMENT_SERVERCLASS_ST( CASW_Ammo_Sniper_Rifle, DT_ASW_Ammo_Sniper_Rifle )
END_SEND_TABLE()

BEGIN_DATADESC( CASW_Ammo_Sniper_Rifle )
END_DATADESC()

LINK_ENTITY_TO_CLASS( asw_ammo_sniper_rifle, CASW_Ammo_Sniper_Rifle );

void CASW_Ammo_Sniper_Rifle::Spawn( void )
{
	Precache();
	SetModel( "models/swarm/ammo/ammosniperrifle.mdl" );
	BaseClass::Spawn();
	m_iAmmoIndex = GetAmmoDef()->Index( "ASW_SNIPER" );
}


void CASW_Ammo_Sniper_Rifle::Precache( void )
{
	PrecacheModel( "models/swarm/ammo/ammosniperrifle.mdl" );
}

void CASW_Ammo_Sniper_Rifle::ActivateUseIcon( CASW_Inhabitable_NPC *pNPC, int nHoldType )
{
	if ( nHoldType == ASW_USE_HOLD_START )
		return;

	if ( ASW_GiveAmmo( pNPC, asw_ammo_count_sniper_rifle.GetInt(), "ASW_SNIPER", this) )
	{
		UTIL_Remove( this );
		return;
	}
}

//-------------
// Heavy Rifle Ammo
//-------------

IMPLEMENT_SERVERCLASS_ST( CASW_Ammo_Heavy_Rifle, DT_ASW_Ammo_Heavy_Rifle )
END_SEND_TABLE()

BEGIN_DATADESC( CASW_Ammo_Heavy_Rifle )
DEFINE_KEYFIELD( m_bAddSecondary, FIELD_BOOLEAN, "AddSecondary" ),
END_DATADESC()

LINK_ENTITY_TO_CLASS( asw_ammo_heavy_rifle, CASW_Ammo_Heavy_Rifle );

void CASW_Ammo_Heavy_Rifle::Spawn( void )
{
	Precache();
	SetModel( "models/swarm/ammo/ammohvyrifle.mdl" );
	BaseClass::Spawn();
	m_iAmmoIndex = GetAmmoDef()->Index( "ASW_HR" );
}


void CASW_Ammo_Heavy_Rifle::Precache( void )
{
	PrecacheModel( "models/swarm/ammo/ammohvyrifle.mdl" );
}

void CASW_Ammo_Heavy_Rifle::ActivateUseIcon( CASW_Inhabitable_NPC *pNPC, int nHoldType )
{
	if ( nHoldType == ASW_USE_HOLD_START )
		return;

	if ( ASW_GiveAmmo( pNPC, asw_ammo_count_heavy_rifle.GetInt(), "ASW_HR", this ) )
	{
		if ( m_bAddSecondary )
		{
			ASW_GiveSecondaryAmmo( pNPC, { CLASS_ASW_HEAVY_RIFLE } );
		}

		UTIL_Remove( this );
		return;
	}
}
