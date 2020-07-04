//========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose:		m16 - assault rifle
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
#include "rumble_shared.h"
#include "grenade_m16.h"
#include "particle_parse.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

//extern ConVar	sk_plr_dmg_m16_grenade;
//ConVar	sk_plr_dmg_m16_grenade("sk_plr_dmg_m16_grenade", "85");


//ConVar dev_m16_rpm("dev_m16_rpm", "650", FCVAR_CHEAT);//825
//ConVar dev_m16_bfcr("dev_m16_bfcr","0.5");
ConVar dev_m16_vector_cone("dev_m16_vector_cone", "0", FCVAR_CHEAT);
ConVar dev_m16_grenade_velocity("dev_m16_grenade_velocity", "1100", FCVAR_CHEAT);
//ConVar dev_m16_forward("dev_m16_forward","40");
//ConVar dev_m16_right("dev_m16_right","0");
//ConVar dev_m16_up("dev_m16_up","0");

//-----------------------------------------------------------------------------
// CWeaponM16
//-----------------------------------------------------------------------------

class CWeaponM16 : public CHLSelectFireMachineGun
{
	DECLARE_CLASS( CWeaponM16, CHLSelectFireMachineGun );
public:

	CWeaponM16();

	void	PrimaryAttack( void );
	//void	FirePrimaryAttackBullet( void );
	void	SecondaryAttack( void );

	//void	BurstFireThink( void );

	float	WeaponAutoAimScale()	{ return 0.6f; }

	//void	ItemPostFrame( void );

	void	AddViewKick( void );

	void	Precache(void);

	float	GetFireRate( void );
	Vector GetM16BulletSpread( void );

	//virtual int		GetBurstFireSize( void ) { return 3; };
	//float			GetBurstFireCycleRate( void ){return dev_m16_bfcr.GetFloat(); };


	bool Deploy(void);

	int CapabilitiesGet( void ) { return bits_CAP_WEAPON_RANGE_ATTACK1; }
	void FireNPCPrimaryAttack( CBaseCombatCharacter *pOperator, Vector &vecShootOrigin, Vector &vecShootDir );
	void Operator_HandleAnimEvent( animevent_t *pEvent, CBaseCombatCharacter *pOperator );

	//int m_iBurstNum;

private:
	int		m_nNumShotsFired;
	float	m_flLastAttackTime;

	DECLARE_SERVERCLASS();
	DECLARE_DATADESC();
	DECLARE_ACTTABLE();
};

LINK_ENTITY_TO_CLASS( weapon_m16, CWeaponM16 );

PRECACHE_WEAPON_REGISTER( weapon_m16 );

IMPLEMENT_SERVERCLASS_ST( CWeaponM16, DT_WeaponM16 )
END_SEND_TABLE()

BEGIN_DATADESC( CWeaponM16 )
//DEFINE_FIELD( m_iBurstNum,		FIELD_INTEGER ),
//DEFINE_FUNCTION( BurstFireThink ),
DEFINE_FIELD(m_nNumShotsFired, FIELD_INTEGER),
DEFINE_FIELD(m_flLastAttackTime, FIELD_TIME),
END_DATADESC()

acttable_t	CWeaponM16::m_acttable[] = 
{
	{ ACT_RANGE_ATTACK1,			ACT_RANGE_ATTACK_AR2,			true },
	{ ACT_RELOAD,					ACT_RELOAD_SMG1,				true },
	{ ACT_IDLE,						ACT_IDLE_SMG1,					true },
	{ ACT_IDLE_ANGRY,				ACT_IDLE_ANGRY_SMG1,			true },

	{ ACT_WALK,						ACT_WALK_RIFLE,					true },
	{ ACT_WALK_AIM,					ACT_WALK_AIM_RIFLE,				true  },
	
// Readiness activities (not aiming)
	{ ACT_IDLE_RELAXED,				ACT_IDLE_SMG1_RELAXED,			false },//never aims
	{ ACT_IDLE_STIMULATED,			ACT_IDLE_SMG1_STIMULATED,		false },
	{ ACT_IDLE_AGITATED,			ACT_IDLE_ANGRY_SMG1,			false },//always aims

	{ ACT_WALK_RELAXED,				ACT_WALK_RIFLE_RELAXED,			false },//never aims
	{ ACT_WALK_STIMULATED,			ACT_WALK_RIFLE_STIMULATED,		false },
	{ ACT_WALK_AGITATED,			ACT_WALK_AIM_RIFLE,				false },//always aims

	{ ACT_RUN_RELAXED,				ACT_RUN_RIFLE_RELAXED,			false },//never aims
	{ ACT_RUN_STIMULATED,			ACT_RUN_RIFLE_STIMULATED,		false },
	{ ACT_RUN_AGITATED,				ACT_RUN_AIM_RIFLE,				false },//always aims

// Readiness activities (aiming)
	{ ACT_IDLE_AIM_RELAXED,			ACT_IDLE_SMG1_RELAXED,			false },//never aims	
	{ ACT_IDLE_AIM_STIMULATED,		ACT_IDLE_AIM_RIFLE_STIMULATED,	false },
	{ ACT_IDLE_AIM_AGITATED,		ACT_IDLE_ANGRY_SMG1,			false },//always aims

	{ ACT_WALK_AIM_RELAXED,			ACT_WALK_RIFLE_RELAXED,			false },//never aims
	{ ACT_WALK_AIM_STIMULATED,		ACT_WALK_AIM_RIFLE_STIMULATED,	false },
	{ ACT_WALK_AIM_AGITATED,		ACT_WALK_AIM_RIFLE,				false },//always aims

	{ ACT_RUN_AIM_RELAXED,			ACT_RUN_RIFLE_RELAXED,			false },//never aims
	{ ACT_RUN_AIM_STIMULATED,		ACT_RUN_AIM_RIFLE_STIMULATED,	false },
	{ ACT_RUN_AIM_AGITATED,			ACT_RUN_AIM_RIFLE,				false },//always aims
//End readiness activities

	{ ACT_WALK_AIM,					ACT_WALK_AIM_RIFLE,				true },
	{ ACT_WALK_CROUCH,				ACT_WALK_CROUCH_RIFLE,			true },
	{ ACT_WALK_CROUCH_AIM,			ACT_WALK_CROUCH_AIM_RIFLE,		true },
	{ ACT_RUN,						ACT_RUN_RIFLE,					true },
	{ ACT_RUN_AIM,					ACT_RUN_AIM_RIFLE,				true },
	{ ACT_RUN_CROUCH,				ACT_RUN_CROUCH_RIFLE,			true },
	{ ACT_RUN_CROUCH_AIM,			ACT_RUN_CROUCH_AIM_RIFLE,		true },
	{ ACT_GESTURE_RANGE_ATTACK1,	ACT_GESTURE_RANGE_ATTACK_SMG1,	true },
	{ ACT_RANGE_ATTACK1_LOW,		ACT_RANGE_ATTACK_SMG1_LOW,		true },
	{ ACT_COVER_LOW,				ACT_COVER_SMG1_LOW,				false },
	{ ACT_RANGE_AIM_LOW,			ACT_RANGE_AIM_SMG1_LOW,			false },
	{ ACT_RELOAD_LOW,				ACT_RELOAD_SMG1_LOW,			false },
	{ ACT_GESTURE_RELOAD,			ACT_GESTURE_RELOAD_SMG1,		true },
};

IMPLEMENT_ACTTABLE(CWeaponM16);

//-----------------------------------------------------------------------------
// Purpose: Constructor
//-----------------------------------------------------------------------------
CWeaponM16::CWeaponM16( void )
{
	m_bReloadsSingly	= false;
	m_bFiresUnderwater	= false;
}

void CWeaponM16::Precache(void)
{
	PrecacheParticleSystem("weapon_muzzle_flash_assaultrifle");
	PrecacheParticleSystem("weapon_muzzle_smoke");
	BaseClass::Precache();
}

void CWeaponM16::PrimaryAttack( void )
{
	if ((gpGlobals->curtime - m_flLastAttackTime) > GetFireRate())
	{
		m_nNumShotsFired = 0;
	}
	else
	{
		m_nNumShotsFired++;
	}
	
	//DevMsg("bullet\n");
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

	

	m_flLastAttackTime = gpGlobals->curtime;

	WeaponSound( SINGLE );
//	pPlayer->DoMuzzleFlash();

	SendWeaponAnim( ACT_VM_PRIMARYATTACK );
	pPlayer->SetAnimation( PLAYER_ATTACK1 );

	m_flNextPrimaryAttack = gpGlobals->curtime + GetFireRate();
	//m_flNextSecondaryAttack = gpGlobals->curtime + 0.75;

	m_iClip1--;

	Vector vecSrc		= pPlayer->Weapon_ShootPosition();
	Vector vecAiming	= pPlayer->GetAutoaimVector( AUTOAIM_SCALE_DEFAULT );	

	pPlayer->FireBullets( 1, vecSrc, vecAiming, /*GetBulletSpread()*/GetM16BulletSpread(), MAX_TRACE_LENGTH, m_iPrimaryAmmoType, 0 );

	pPlayer->SetMuzzleFlashTime( gpGlobals->curtime + 0.5 );

	//Disorient the player
	//AddViewKick();

	//QAngle angles = pPlayer->GetLocalAngles();

	//angles.x += random->RandomInt( -1, 1 );
	//angles.y += random->RandomInt( -1, 1 );
	//angles.z = 0;

	//pPlayer->SnapEyeAngles( angles );

	pPlayer->ViewPunch( QAngle( -0.5, 0, 0 ) );
	//pPlayer->ViewPunch( QAngle( -8, random->RandomFloat( -2, 2 ), 0 ) );

	CSoundEnt::InsertSound( SOUND_COMBAT, GetAbsOrigin(), 600, 0.2, GetOwner() );

	if ( !m_iClip1 && pPlayer->GetAmmoCount( m_iPrimaryAmmoType ) <= 0 )
	{
		// HEV suit - indicate out of ammo condition
		pPlayer->SetSuitUpdate( "!HEV_AMO0", FALSE, 0 ); 
	}

	DispatchParticleEffect("weapon_muzzle_flash_assaultrifle", PATTACH_POINT_FOLLOW, pPlayer->GetViewModel(), "muzzle", true);
	if (m_nNumShotsFired >= 3){
		DispatchParticleEffect("weapon_muzzle_smoke", PATTACH_POINT_FOLLOW, pPlayer->GetViewModel(), "muzzle", true);
	}
}

void CWeaponM16::SecondaryAttack( void )
{
	//CBasePlayer *pPlayer = ToBasePlayer( GetOwner() );
	//Vector vecSrc		= pPlayer->Weapon_ShootPosition();
	//Vector vecAiming	= pPlayer->GetAutoaimVector( AUTOAIM_SCALE_DEFAULT );
	//QAngle angAiming;
	//VectorAngles( vecAiming, angAiming );
	//
	//Vector forward;
	//Vector right; 
	//Vector up;
	//AngleVectors(pPlayer->EyeAngles(),&forward,&right,&up);

	//vecSrc = vecSrc + (dev_m16_right.GetFloat() * right);
	//vecSrc = vecSrc + (dev_m16_up.GetFloat() * up);
	//vecSrc = vecSrc + (dev_m16_forward.GetFloat() * forward);

	////CPitdroneSpike *pSpike;
	////pSpike->SpikeCreate( vecSrc, angAiming, pPlayer );
	////pSpike->SetAbsVelocity( vecAiming * 1500.0f );

	//CPitdroneSpike *pSpike = CPitdroneSpike::SpikeCreate( vecSrc, angAiming, pPlayer );
	//if ( pSpike )
	//	pSpike->SetAbsVelocity( vecAiming * 1500.0f );

	//m_flNextSecondaryAttack = gpGlobals->curtime + 0.75;
	
	
	// modified smg1 grenade
//	Msg("in CWeaponM16::SecondaryAttack\n");
	// Only the player fires this way so we can cast
	CBasePlayer *pPlayer = ToBasePlayer( GetOwner() );
	
	if ( pPlayer == NULL )
		return;

	//Must have ammo
	if ( ( pPlayer->GetAmmoCount( m_iSecondaryAmmoType ) <= 0 ) || ( pPlayer->GetWaterLevel() == 3 ) )
	{
		SendWeaponAnim( ACT_VM_DRYFIRE );
		BaseClass::WeaponSound( EMPTY );
		m_flNextSecondaryAttack = gpGlobals->curtime + 0.5f;
		return;
	}

	if( m_bInReload )
		m_bInReload = false;

	// MUST call sound before removing a round from the clip of a CMachineGun
	BaseClass::WeaponSound( WPN_DOUBLE );

	pPlayer->RumbleEffect( RUMBLE_357, 0, RUMBLE_FLAGS_NONE );


	Vector vecSrc = pPlayer->Weapon_ShootPosition();
	Vector	vecThrow;
	// Don't autoaim on grenade tosses
	AngleVectors( pPlayer->EyeAngles() + pPlayer->GetPunchAngle(), &vecThrow );
	VectorScale( vecThrow, dev_m16_grenade_velocity.GetFloat(), vecThrow );
	
	//Create the grenade
	QAngle angles;
	VectorAngles( vecThrow, angles );
	CGrenadeM16 *pGrenade = (CGrenadeM16*)Create( "grenade_m16", vecSrc, angles, pPlayer );
	pGrenade->SetAbsVelocity( vecThrow );

	pGrenade->SetLocalAngularVelocity( RandomAngle( -400, 400 ) );
	pGrenade->SetMoveType( MOVETYPE_FLYGRAVITY, MOVECOLLIDE_FLY_BOUNCE ); 
	pGrenade->SetThrower( GetOwner() );
	//pGrenade->SetDamage( sk_plr_dmg_m16_grenade.GetFloat() );

	SendWeaponAnim( ACT_VM_SECONDARYATTACK );

	CSoundEnt::InsertSound( SOUND_COMBAT, GetAbsOrigin(), 1000, 0.2, GetOwner(), SOUNDENT_CHANNEL_WEAPON );

	// player "shoot" animation
	pPlayer->SetAnimation( PLAYER_ATTACK1 );

	// Decrease ammo
	pPlayer->RemoveAmmo( 1, m_iSecondaryAmmoType );

	// Can shoot again immediately
	m_flNextPrimaryAttack = gpGlobals->curtime + 0.5f;

	// Can blow up after a short delay (so have time to release mouse button)
	m_flNextSecondaryAttack = gpGlobals->curtime + 1.0f;

	// Register a muzzleflash for the AI.
	pPlayer->SetMuzzleFlashTime( gpGlobals->curtime + 0.5 );	
	
	//Disorient the player
	QAngle angles2 = pPlayer->GetLocalAngles();

	angles2.x += random->RandomInt( -1, 1 );
	angles2.y += random->RandomInt( -1, 1 );
	angles2.z = 0;

	pPlayer->SnapEyeAngles( angles2 );

	pPlayer->ViewPunch( QAngle( -8, random->RandomFloat( -2, 2 ), 0 ) );

	m_iSecondaryAttacks++;
	gamestats->Event_WeaponFired( pPlayer, false, GetClassname() );
	
}

float	CWeaponM16::GetFireRate( void )
{
	return 60.0/650.0;
}

Vector CWeaponM16::GetM16BulletSpread( void )
{
	float spread = sin( DEG2RAD( 1.0f ));
	return Vector(spread,spread,spread);
}

bool CWeaponM16::Deploy( void )
{
	// Forget about any bursts this weapon was firing when holstered
	//m_iBurstNum = 0;
	return BaseClass::Deploy();
}

////-----------------------------------------------------------------------------
//// Purpose: 
////
////
////-----------------------------------------------------------------------------
//void CWeaponM16::BurstFireThink( void )
//{
//	//DevMsg("Hello!\n");
//	FirePrimaryAttackBullet();
//
//	m_iBurstNum--;
//
//	if( m_iBurstNum == 0 )
//	{
//		// The burst is over!
//		SetThink(NULL);
//
//		// idle immediately to stop the firing animation
//		SetWeaponIdleTime( gpGlobals->curtime );
//		return;
//	}
//
//	SetNextThink( gpGlobals->curtime + GetFireRate() );
//}

////-----------------------------------------------------------------------------
//// Purpose: 
////
////
////-----------------------------------------------------------------------------
//void CWeaponM16::ItemPostFrame( void )
//{
//	//DevMsg("in CWeaponM16::ItemPostFrame!!!\n");
//	CBasePlayer *pOwner = ToBasePlayer( GetOwner() );
//	
//	if ( pOwner == NULL )
//		return;
//
//	// Debounce the recoiling counter
//	if ( ( pOwner->m_nButtons & IN_ATTACK ) == false )
//	{
//		m_nShotsFired = 0;
//	}
//
//
//	//Track the duration of the fire
//	//FIXME: Check for IN_ATTACK2 as well?
//	//FIXME: What if we're calling ItemBusyFrame?
//	m_fFireDuration = ( pOwner->m_nButtons & IN_ATTACK ) ? ( m_fFireDuration + gpGlobals->frametime ) : 0.0f;
//
//	if ( UsesClipsForAmmo1() )
//	{
//		CheckReload();
//	}
//
//	bool bFired = false;
//
//	// Secondary attack has priority
//	if ((pOwner->m_nButtons & IN_ATTACK2) && (m_flNextSecondaryAttack <= gpGlobals->curtime))
//	{
//		if (UsesSecondaryAmmo() && pOwner->GetAmmoCount(m_iSecondaryAmmoType)<=0 )
//		{
//			if (m_flNextEmptySoundTime < gpGlobals->curtime)
//			{
//				WeaponSound(EMPTY);
//				m_flNextSecondaryAttack = m_flNextEmptySoundTime = gpGlobals->curtime + 0.5;
//			}
//		}
//		else if (pOwner->GetWaterLevel() == 3 && m_bAltFiresUnderwater == false)
//		{
//			// This weapon doesn't fire underwater
//			WeaponSound(EMPTY);
//			m_flNextPrimaryAttack = gpGlobals->curtime + 0.2;
//			return;
//		}
//		else
//		{
//			// FIXME: This isn't necessarily true if the weapon doesn't have a secondary fire!
//			// For instance, the crossbow doesn't have a 'real' secondary fire, but it still 
//			// stops the crossbow from firing on the 360 if the player chooses to hold down their
//			// zoom button. (sjb) Orange Box 7/25/2007
//#if !defined(CLIENT_DLL)
//			if( !IsX360() || !ClassMatches("weapon_crossbow") )
//#endif
//			{
//				bFired = true;
//			}
//
//			SecondaryAttack();
//
//			// Secondary ammo doesn't have a reload animation
//			if ( UsesClipsForAmmo2() )
//			{
//				// reload clip2 if empty
//				if (m_iClip2 < 1)
//				{
//					pOwner->RemoveAmmo( 1, m_iSecondaryAmmoType );
//					m_iClip2 = m_iClip2 + 1;
//				}
//			}
//		}
//	}
//	
//	if ( !bFired && (pOwner->m_nButtons & IN_ATTACK) && (m_flNextPrimaryAttack <= gpGlobals->curtime))
//	{
//		// Clip empty? Or out of ammo on a no-clip weapon?
//		if ( !IsMeleeWeapon() &&  
//			(( UsesClipsForAmmo1() && m_iClip1 <= 0) || ( !UsesClipsForAmmo1() && pOwner->GetAmmoCount(m_iPrimaryAmmoType)<=0 )) )
//		{
//			HandleFireOnEmpty();
//		}
//		else if (pOwner->GetWaterLevel() == 3 && m_bFiresUnderwater == false)
//		{
//			DevMsg("This weapon doesn't fire underwater\n");
//			WeaponSound(EMPTY);
//			m_flNextPrimaryAttack = gpGlobals->curtime + 0.2;
//			return;
//		}
//		else
//		{
//			//NOTENOTE: There is a bug with this code with regards to the way machine guns catch the leading edge trigger
//			//			on the player hitting the attack key.  It relies on the gun catching that case in the same frame.
//			//			However, because the player can also be doing a secondary attack, the edge trigger may be missed.
//			//			We really need to hold onto the edge trigger and only clear the condition when the gun has fired its
//			//			first shot.  Right now that's too much of an architecture change -- jdw
//			
//			// If the firing button was just pressed, or the alt-fire just released, reset the firing time
//			if ( ( pOwner->m_afButtonPressed & IN_ATTACK ) || ( pOwner->m_afButtonReleased & IN_ATTACK2 ) )
//			{
//				 m_flNextPrimaryAttack = gpGlobals->curtime;
//			}
//
//			PrimaryAttack();
//		}
//	}
//
//	// -----------------------
//	//  Reload pressed / Clip Empty
//	// -----------------------
//	if ( pOwner->m_nButtons & IN_RELOAD && UsesClipsForAmmo1() && !m_bInReload ) 
//	{
//		// reload when reload is pressed, or if no buttons are down and weapon is empty.
//		Reload();
//		m_fFireDuration = 0.0f;
//	}
//
//	// -----------------------
//	//  No buttons down
//	// -----------------------
//	if (!((pOwner->m_nButtons & IN_ATTACK) || (pOwner->m_nButtons & IN_ATTACK2) || (pOwner->m_nButtons & IN_RELOAD)))
//	{
//		// no fire buttons down or reloading
//		if ( !ReloadOrSwitchWeapons() && ( m_bInReload == false ) )
//		{
//			WeaponIdle();
//		}
//	}
//}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CWeaponM16::AddViewKick( void )
{
	//DevMsg("view kick\n");
	#define	EASY_DAMPEN			0.5f
	#define	MAX_VERTICAL_KICK	12.0f	//Degrees
	#define	SLIDE_LIMIT			0.35f	//Seconds
	
	//Get the view kick
	CBasePlayer *pPlayer = ToBasePlayer( GetOwner() );

	if (!pPlayer)
		return;

//	float flDuration = m_fFireDuration;


	DoMachineGunKick(pPlayer, EASY_DAMPEN, MAX_VERTICAL_KICK, 0.01, SLIDE_LIMIT);
}

//-----------------------------------------------------------------------------
// Purpose:
// Input  :
// Output :
//-----------------------------------------------------------------------------
void CWeaponM16::Operator_HandleAnimEvent( animevent_t *pEvent, CBaseCombatCharacter *pOperator )
{
	switch( pEvent->event )
	{
		DevMsg("event: %d\n",pEvent->event);
		//case EVENT_WEAPON_SHOTGUN_FIRE:
		//{
		//	FireNPCPrimaryAttack( pOperator, false );
		//}
		//break;
		case EVENT_WEAPON_AR2:
		case EVENT_WEAPON_SMG1:
		{
			Vector vecShootOrigin, vecShootDir;
			QAngle angDiscard;

			// Support old style attachment point firing
			if ((pEvent->options == NULL) || (pEvent->options[0] == '\0') || (!pOperator->GetAttachment(pEvent->options, vecShootOrigin, angDiscard)))
			{
				vecShootOrigin = pOperator->Weapon_ShootPosition();
			}

			CAI_BaseNPC *npc = pOperator->MyNPCPointer();
			ASSERT( npc != NULL );
			vecShootDir = npc->GetActualShootTrajectory( vecShootOrigin );
			//DevMsg("HandleAnimEvent\n");
			FireNPCPrimaryAttack( pOperator, vecShootOrigin, vecShootDir );
		}
		break;


		default:
			CBaseCombatWeapon::Operator_HandleAnimEvent( pEvent, pOperator );
			break;
	}
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CWeaponM16::FireNPCPrimaryAttack( CBaseCombatCharacter *pOperator, Vector &vecShootOrigin, Vector &vecShootDir )
{
	// FIXME: use the returned number of bullets to account for >10hz firerate
	WeaponSoundRealtime( SINGLE_NPC );

	CSoundEnt::InsertSound( SOUND_COMBAT|SOUND_CONTEXT_GUNFIRE, pOperator->GetAbsOrigin(), SOUNDENT_VOLUME_MACHINEGUN, 0.2, pOperator, SOUNDENT_CHANNEL_WEAPON, pOperator->GetEnemy() );
	Vector vColor(0,0,255);
	//NDebugOverlay::Line( vecShootOrigin, vecShootOrigin + (vecShootDir * 4000.0f), vColor.x, vColor.y, vColor.z, true, .5 );
	FireBulletsInfo_t info;
	info.m_iShots = 1;
	info.m_vecSrc = vecShootOrigin;
	info.m_vecDirShooting = vecShootDir;
	info.m_vecSpread = VECTOR_CONE_PRECALCULATED;
	info.m_flDistance = MAX_TRACE_LENGTH;
	info.m_iAmmoType = m_iPrimaryAmmoType;
	info.m_iTracerFreq = 2;
	info.m_flDamage = 2.0f;
	//info.m_pAttacker = pAttacker;
	//info.m_nFlags = bFirstShotAccurate ? FIRE_BULLETS_FIRST_SHOT_ACCURATE : 0;
	//info.m_bPrimaryAttack = bPrimaryAttack;

	pOperator->FireBullets( info );


	pOperator->DoMuzzleFlash();
	m_iClip1 = m_iClip1 - 1;
}