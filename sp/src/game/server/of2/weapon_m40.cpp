//========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose:		M40 - sniper rifle
//
// $NoKeywords: $
//=============================================================================//

#include "cbase.h"
#include "NPCEvent.h"
#include "basehlcombatweapon.h"
#include "basecombatcharacter.h"
#include "AI_BaseNPC.h"
#include "player.h"
#include "gamerules.h"
#include "in_buttons.h"
#include "soundent.h"
#include "game.h"
#include "vstdlib/random.h"
#include "engine/IEngineSound.h"
#include "te_effect_dispatch.h"
#include "gamestats.h"
#include "particle_parse.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

ConVar dev_m40_rpm("dev_m40_rpm", "60", FCVAR_CHEAT);
ConVar dev_m40_vector_cone("dev_m40_vector_cone", "0", FCVAR_CHEAT);//originally 1

//-----------------------------------------------------------------------------
// CWeaponM40
//-----------------------------------------------------------------------------

class CWeaponM40 : public CBaseHLCombatWeapon
{
	DECLARE_CLASS( CWeaponM40, CBaseHLCombatWeapon );
public:

	CWeaponM40( void );

	void	PrimaryAttack( void );
	float	GetFireRate( void );
	Vector	GetM40BulletSpread( void );

	void	ItemBusyFrame();
	void	ItemPostFrame();
	void	Precache();

	void	StopEffects();
	void	ToggleZoom();
	bool    m_bDrawViewmodel = true;

	float	WeaponAutoAimScale()	{ return 0.6f; }
	bool	IsWeaponZoomed() { return m_bInZoom; }

	void	CheckZoomToggle();
	bool	Holster( CBaseCombatWeapon *pSwitchingTo = NULL );

	bool				m_bInZoom;
	bool	m_bMustReload;
private:
	int		m_nNumShotsFired;
	DECLARE_SERVERCLASS();
	DECLARE_DATADESC();
};

LINK_ENTITY_TO_CLASS( weapon_m40, CWeaponM40 );

PRECACHE_WEAPON_REGISTER( weapon_m40 );

IMPLEMENT_SERVERCLASS_ST( CWeaponM40, DT_WeaponM40 )
END_SEND_TABLE()

BEGIN_DATADESC( CWeaponM40 )
END_DATADESC()

//-----------------------------------------------------------------------------
// Purpose: Constructor
//-----------------------------------------------------------------------------
CWeaponM40::CWeaponM40( void )
{
	m_bReloadsSingly	= false;
	m_bFiresUnderwater	= false;
}

void CWeaponM40::Precache(void)
{
	PrecacheParticleSystem("weapon_muzzle_flash_awp");
	PrecacheParticleSystem("weapon_muzzle_smoke");
	BaseClass::Precache();
}

//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
void CWeaponM40::PrimaryAttack( void )
{
	// Only the player fires this way so we can cast
	CBasePlayer *pPlayer = ToBasePlayer( GetOwner() );

	if ( !pPlayer )
	{
		return;
	}

	if ( m_iClip1 <= 0 )
	{
		if ( !m_bFireOnEmpty )
		{
			Reload();
		}
		else
		{
			WeaponSound( EMPTY );
			m_flNextPrimaryAttack = 0.15;
		}

		return;
	}

	m_iPrimaryAttacks++;
	gamestats->Event_WeaponFired( pPlayer, true, GetClassname() );

	WeaponSound( SINGLE );
//	pPlayer->DoMuzzleFlash();
	DispatchParticleEffect("weapon_muzzle_flash_awp", PATTACH_POINT_FOLLOW, pPlayer->GetViewModel(), "muzzle", true);
	if (m_nNumShotsFired >= 2){
		DispatchParticleEffect("weapon_muzzle_smoke", PATTACH_POINT_FOLLOW, pPlayer->GetViewModel(), "muzzle", true);
	}
	SendWeaponAnim( ACT_VM_PRIMARYATTACK );
	pPlayer->SetAnimation( PLAYER_ATTACK1 );

	m_flNextPrimaryAttack = gpGlobals->curtime + GetFireRate();
	m_flNextSecondaryAttack = gpGlobals->curtime + 0.75;

	m_iClip1--;

	Vector vecSrc		= pPlayer->Weapon_ShootPosition();
	Vector vecAiming	= pPlayer->GetAutoaimVector( AUTOAIM_SCALE_DEFAULT );	

	pPlayer->FireBullets( 1, vecSrc, vecAiming, GetM40BulletSpread(), MAX_TRACE_LENGTH, m_iPrimaryAmmoType, 0 );

	pPlayer->SetMuzzleFlashTime( gpGlobals->curtime + 0.5 );



	//Disorient the player
	QAngle angles = pPlayer->GetLocalAngles();

	angles.x += random->RandomInt( -1, 1 );
	angles.y += random->RandomInt( -1, 1 );
	angles.z = 0;

	pPlayer->SnapEyeAngles( angles );

	pPlayer->ViewPunch( QAngle( -8, random->RandomFloat( -2, 2 ), 0 ) );

	CSoundEnt::InsertSound( SOUND_COMBAT, GetAbsOrigin(), 600, 0.2, GetOwner() );

	if ( !m_iClip1 && pPlayer->GetAmmoCount( m_iPrimaryAmmoType ) <= 0 )
	{
		// HEV suit - indicate out of ammo condition
		pPlayer->SetSuitUpdate( "!HEV_AMO0", FALSE, 0 ); 
	}
}

float	CWeaponM40::GetFireRate( void )
{
	return 60.0/dev_m40_rpm.GetFloat();
}

Vector CWeaponM40::GetM40BulletSpread( void )
{
	float spread = sin( DEG2RAD( dev_m40_vector_cone.GetFloat() /2.0f ));
	return Vector(spread,spread,spread);
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CWeaponM40::CheckZoomToggle( void )
{
	CBasePlayer *pPlayer = ToBasePlayer( GetOwner() );
	
	if ( pPlayer->m_afButtonPressed & IN_ATTACK2 )
	{
			ToggleZoom();
	}
	
	//if ( pPlayer->m_afButtonReleased & IN_ATTACK2 )
	//{
	//	ToggleZoom();
	//}
}

void CWeaponM40::ItemBusyFrame( void )
{
	CheckZoomToggle();
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CWeaponM40::ItemPostFrame( void )
{
	// Allow zoom toggling
	CheckZoomToggle();

	if ( m_bMustReload && HasWeaponIdleTimeElapsed() )
	{
		Reload();
	}

	BaseClass::ItemPostFrame();
}

//-----------------------------------------------------------------------------
// Purpose: Stop all zooming and special effects on the viewmodel
//-----------------------------------------------------------------------------
void CWeaponM40::StopEffects( void )
{
	// Stop zooming
	if ( m_bInZoom )
	{
		ToggleZoom();
	}

	// Turn off our sprites
	//SetChargerState( CHARGER_STATE_OFF );
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CWeaponM40::ToggleZoom( void )
{
	CBasePlayer *pPlayer = ToBasePlayer( GetOwner() );
	
	if ( pPlayer == NULL )
		return;

	if (m_bInZoom)
	{
		if (pPlayer->SetFOV(this, 0, 0.2f))
		{
			m_bInZoom = false;
			m_bDrawViewmodel = true;
			// Send a message to hide the scope
			CSingleUserRecipientFilter filter(pPlayer);
			UserMessageBegin(filter, "ShowScope");
			WRITE_BYTE(0);
			MessageEnd();
		}
	}
	else
	{
		if (pPlayer->SetFOV(this, 20, 0.1f))
		{
			m_bInZoom = true;
			m_bDrawViewmodel = false;
			// Send a message to Show the scope
			CSingleUserRecipientFilter filter(pPlayer);
			UserMessageBegin(filter, "ShowScope");
			WRITE_BYTE(1);
			MessageEnd();
		}
	}
}

//-----------------------------------------------------------------------------
// Purpose: 
// Input  : *pSwitchingTo - 
// Output : Returns true on success, false on failure.
//-----------------------------------------------------------------------------
bool CWeaponM40::Holster( CBaseCombatWeapon *pSwitchingTo )
{
	StopEffects();
	return BaseClass::Holster( pSwitchingTo );
}