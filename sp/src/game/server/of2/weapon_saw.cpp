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
#include "particle_parse.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

//extern ConVar	sk_plr_dmg_m16_grenade;
//ConVar	sk_plr_dmg_m16_grenade("sk_plr_dmg_m16_grenade", "85");


//ConVar dev_m16_rpm("dev_m16_rpm", "650", FCVAR_CHEAT);//825
//ConVar dev_m16_bfcr("dev_m16_bfcr","0.5");
//ConVar dev_m16_vector_cone("dev_m16_vector_cone", "0", FCVAR_CHEAT);
//ConVar dev_m16_grenade_velocity("dev_m16_grenade_velocity", "1100", FCVAR_CHEAT);
//ConVar dev_m16_forward("dev_m16_forward","40");
//ConVar dev_m16_right("dev_m16_right","0");
//ConVar dev_m16_up("dev_m16_up","0");

//-----------------------------------------------------------------------------
// CWeaponSAW
//-----------------------------------------------------------------------------

class CWeaponSAW : public CHLSelectFireMachineGun
{
	DECLARE_CLASS(CWeaponSAW, CHLSelectFireMachineGun);
public:

	CWeaponSAW();

	void	PrimaryAttack(void);
	//void	FirePrimaryAttackBullet( void );
	void	SecondaryAttack(void);

	void	Precache(void);
	//void	BurstFireThink( void );

	float	WeaponAutoAimScale()	{ return 0.6f; }

	void	ItemPostFrame( void );

	void	AddViewKick(void);

	float	GetFireRate(void);
	Vector GetSAWBulletSpread(void);

	//virtual int		GetBurstFireSize( void ) { return 3; };
	//float			GetBurstFireCycleRate( void ){return dev_m16_bfcr.GetFloat(); };


	bool Deploy(void);

	int CapabilitiesGet(void) { return bits_CAP_WEAPON_RANGE_ATTACK1; }
	void FireNPCPrimaryAttack(CBaseCombatCharacter *pOperator, Vector &vecShootOrigin, Vector &vecShootDir);
	void Operator_HandleAnimEvent(animevent_t *pEvent, CBaseCombatCharacter *pOperator);

	

	//int m_iBurstNum;

	DECLARE_SERVERCLASS();
	DECLARE_DATADESC();
	DECLARE_ACTTABLE();

private:
	int		m_nNumShotsFired;
};

LINK_ENTITY_TO_CLASS(weapon_saw, CWeaponSAW);

PRECACHE_WEAPON_REGISTER(weapon_saw);

IMPLEMENT_SERVERCLASS_ST(CWeaponSAW, DT_WeaponSAW)
END_SEND_TABLE()

BEGIN_DATADESC(CWeaponSAW)
//DEFINE_FIELD( m_iBurstNum,		FIELD_INTEGER ),
//DEFINE_FUNCTION( BurstFireThink ),
END_DATADESC()

acttable_t	CWeaponSAW::m_acttable[] =
{
	{ ACT_RANGE_ATTACK1, ACT_RANGE_ATTACK_AR2, true },
	{ ACT_RELOAD, ACT_RELOAD_SMG1, true },
	{ ACT_IDLE, ACT_IDLE_SMG1, true },
	{ ACT_IDLE_ANGRY, ACT_IDLE_ANGRY_SMG1, true },

	{ ACT_WALK, ACT_WALK_RIFLE, true },
	{ ACT_WALK_AIM, ACT_WALK_AIM_RIFLE, true },

	// Readiness activities (not aiming)
	{ ACT_IDLE_RELAXED, ACT_IDLE_SMG1_RELAXED, false },//never aims
	{ ACT_IDLE_STIMULATED, ACT_IDLE_SMG1_STIMULATED, false },
	{ ACT_IDLE_AGITATED, ACT_IDLE_ANGRY_SMG1, false },//always aims

	{ ACT_WALK_RELAXED, ACT_WALK_RIFLE_RELAXED, false },//never aims
	{ ACT_WALK_STIMULATED, ACT_WALK_RIFLE_STIMULATED, false },
	{ ACT_WALK_AGITATED, ACT_WALK_AIM_RIFLE, false },//always aims

	{ ACT_RUN_RELAXED, ACT_RUN_RIFLE_RELAXED, false },//never aims
	{ ACT_RUN_STIMULATED, ACT_RUN_RIFLE_STIMULATED, false },
	{ ACT_RUN_AGITATED, ACT_RUN_AIM_RIFLE, false },//always aims

	// Readiness activities (aiming)
	{ ACT_IDLE_AIM_RELAXED, ACT_IDLE_SMG1_RELAXED, false },//never aims	
	{ ACT_IDLE_AIM_STIMULATED, ACT_IDLE_AIM_RIFLE_STIMULATED, false },
	{ ACT_IDLE_AIM_AGITATED, ACT_IDLE_ANGRY_SMG1, false },//always aims

	{ ACT_WALK_AIM_RELAXED, ACT_WALK_RIFLE_RELAXED, false },//never aims
	{ ACT_WALK_AIM_STIMULATED, ACT_WALK_AIM_RIFLE_STIMULATED, false },
	{ ACT_WALK_AIM_AGITATED, ACT_WALK_AIM_RIFLE, false },//always aims

	{ ACT_RUN_AIM_RELAXED, ACT_RUN_RIFLE_RELAXED, false },//never aims
	{ ACT_RUN_AIM_STIMULATED, ACT_RUN_AIM_RIFLE_STIMULATED, false },
	{ ACT_RUN_AIM_AGITATED, ACT_RUN_AIM_RIFLE, false },//always aims
	//End readiness activities

	{ ACT_WALK_AIM, ACT_WALK_AIM_RIFLE, true },
	{ ACT_WALK_CROUCH, ACT_WALK_CROUCH_RIFLE, true },
	{ ACT_WALK_CROUCH_AIM, ACT_WALK_CROUCH_AIM_RIFLE, true },
	{ ACT_RUN, ACT_RUN_RIFLE, true },
	{ ACT_RUN_AIM, ACT_RUN_AIM_RIFLE, true },
	{ ACT_RUN_CROUCH, ACT_RUN_CROUCH_RIFLE, true },
	{ ACT_RUN_CROUCH_AIM, ACT_RUN_CROUCH_AIM_RIFLE, true },
	{ ACT_GESTURE_RANGE_ATTACK1, ACT_GESTURE_RANGE_ATTACK_SMG1, true },
	{ ACT_RANGE_ATTACK1_LOW, ACT_RANGE_ATTACK_SMG1_LOW, true },
	{ ACT_COVER_LOW, ACT_COVER_SMG1_LOW, false },
	{ ACT_RANGE_AIM_LOW, ACT_RANGE_AIM_SMG1_LOW, false },
	{ ACT_RELOAD_LOW, ACT_RELOAD_SMG1_LOW, false },
	{ ACT_GESTURE_RELOAD, ACT_GESTURE_RELOAD_SMG1, true },
};

IMPLEMENT_ACTTABLE(CWeaponSAW);

//-----------------------------------------------------------------------------
// Purpose: Constructor
//-----------------------------------------------------------------------------
CWeaponSAW::CWeaponSAW(void)
{
	m_bReloadsSingly = false;
	m_bFiresUnderwater = false;
}

void CWeaponSAW::Precache(void)
{
	PrecacheParticleSystem("weapon_muzzle_flash_awp");
	PrecacheParticleSystem("weapon_muzzle_smoke");
	BaseClass::Precache();
}

void CWeaponSAW::PrimaryAttack(void)
{
	//DevMsg("bullet\n");
	// Only the player fires this way so we can cast
	CBasePlayer *pPlayer = ToBasePlayer(GetOwner());

	if (!pPlayer)
	{
		return;
	}

	if (m_iClip1 <= 0)
	{
		if (!m_bFireOnEmpty)
		{
			Reload();
		}
		else
		{
			WeaponSound(EMPTY);
			m_flNextPrimaryAttack = 0.15;
		}

		return;
	}

	m_iPrimaryAttacks++;
	gamestats->Event_WeaponFired(pPlayer, true, GetClassname());

	WeaponSound(SINGLE);
	//pPlayer->DoMuzzleFlash();

	SendWeaponAnim(ACT_VM_PRIMARYATTACK);
	pPlayer->SetAnimation(PLAYER_ATTACK1);

	m_flNextPrimaryAttack = gpGlobals->curtime + GetFireRate();
	//m_flNextSecondaryAttack = gpGlobals->curtime + 0.75;

	m_iClip1--;

	Vector vecSrc = pPlayer->Weapon_ShootPosition();
	Vector vecAiming = pPlayer->GetAutoaimVector(AUTOAIM_SCALE_DEFAULT);

	pPlayer->FireBullets(1, vecSrc, vecAiming, /*GetBulletSpread()*/GetSAWBulletSpread(), MAX_TRACE_LENGTH, m_iPrimaryAmmoType, 0);

	pPlayer->SetMuzzleFlashTime(gpGlobals->curtime + 0.5);

	//Disorient the player
	AddViewKick();

	QAngle angles = pPlayer->GetLocalAngles();

	angles.x += random->RandomInt( -1, 1 );
	angles.y += random->RandomInt( -1, 1 );
	angles.z = random->RandomFloat(-0.25, 0.25);

	pPlayer->SnapEyeAngles( angles );

	pPlayer->ViewPunch(QAngle(-0.1, 0, 0));
	//pPlayer->ViewPunch( QAngle( -8, random->RandomFloat( -2, 2 ), 0 ) );

	CSoundEnt::InsertSound(SOUND_COMBAT, GetAbsOrigin(), 600, 0.2, GetOwner());

	DispatchParticleEffect("weapon_muzzle_flash_awp", PATTACH_POINT_FOLLOW, pPlayer->GetViewModel(), "muzzle", true);
	if(m_nNumShotsFired >= 3){
		DispatchParticleEffect("weapon_muzzle_smoke", PATTACH_POINT_FOLLOW, pPlayer->GetViewModel(), "muzzle", true);
	}

//	Vector vecThrow = vecAiming * -50;
//	pPlayer->SetAbsVelocity( vecThrow );

	//if (!m_iClip1 && pPlayer->GetAmmoCount(m_iPrimaryAmmoType) <= 0)
	//{
	//	// HEV suit - indicate out of ammo condition
	//	pPlayer->SetSuitUpdate("!HEV_AMO0", FALSE, 0);
	//}
}

void CWeaponSAW::SecondaryAttack(void)
{
	
}

float CWeaponSAW::GetFireRate(void)
{
	return 60.0 / 800;
}

Vector CWeaponSAW::GetSAWBulletSpread(void)
{
	float spread = sin(DEG2RAD(2.5f));
	return Vector(spread, spread, spread);
}

bool CWeaponSAW::Deploy(void)
{
	// Forget about any bursts this weapon was firing when holstered
	//m_iBurstNum = 0;
	return BaseClass::Deploy();
}


void CWeaponSAW::ItemPostFrame( void )
{
	//DevMsg("in CWeaponSAW::ItemPostFrame!!!\n");
	CBasePlayer *pOwner = ToBasePlayer( GetOwner() );
	
	if ( pOwner == NULL )
		return;

	// Debounce the recoiling counter
	if ( ( pOwner->m_nButtons & IN_ATTACK ) == false )
	{
		m_nShotsFired = 0;
	}


	//Track the duration of the fire
	//FIXME: Check for IN_ATTACK2 as well?
	//FIXME: What if we're calling ItemBusyFrame?
	m_fFireDuration = ( pOwner->m_nButtons & IN_ATTACK ) ? ( m_fFireDuration + gpGlobals->frametime ) : 0.0f;

	if ( UsesClipsForAmmo1() )
	{
		CheckReload();
	}

	bool bFired = false;
	
	if ( !bFired && (pOwner->m_nButtons & IN_ATTACK) && (m_flNextPrimaryAttack <= gpGlobals->curtime))
	{
		// Clip empty? Or out of ammo on a no-clip weapon?
		if ( !IsMeleeWeapon() &&  
			(( UsesClipsForAmmo1() && m_iClip1 <= 0) || ( !UsesClipsForAmmo1() && pOwner->GetAmmoCount(m_iPrimaryAmmoType)<=0 )) )
		{
			HandleFireOnEmpty();
		}
		else if (pOwner->GetWaterLevel() == 3 && m_bFiresUnderwater == false)
		{
			DevMsg("This weapon doesn't fire underwater\n");
			WeaponSound(EMPTY);
			m_flNextPrimaryAttack = gpGlobals->curtime + 0.2;
			return;
		}
		else
		{
			// If the firing button was just pressed, or the alt-fire just released, reset the firing time
			if ( ( pOwner->m_afButtonPressed & IN_ATTACK ) || ( pOwner->m_afButtonReleased & IN_ATTACK2 ) )
			{
				 m_flNextPrimaryAttack = gpGlobals->curtime;
			}

			PrimaryAttack();
		}
	}

	// -----------------------
	//  Reload pressed / Clip Empty
	// -----------------------
	if ( pOwner->m_nButtons & IN_RELOAD && UsesClipsForAmmo1() && !m_bInReload ) 
	{
		// reload when reload is pressed, or if no buttons are down and weapon is empty.
		Reload();
		m_fFireDuration = 0.0f;
	}

	// -----------------------
	//  No buttons down
	// -----------------------
	if (!((pOwner->m_nButtons & IN_ATTACK) || (pOwner->m_nButtons & IN_ATTACK2) || (pOwner->m_nButtons & IN_RELOAD)))
	{
		// no fire buttons down or reloading
		if ( !ReloadOrSwitchWeapons() && ( m_bInReload == false ) )
		{
			WeaponIdle();
		}
	}
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CWeaponSAW::AddViewKick(void)
{
	//DevMsg("view kick\n");
#define	EASY_DAMPEN			0.5f
#define	MAX_VERTICAL_KICK	2.0f	//Degrees
#define	SLIDE_LIMIT			0.35f	//Seconds

	//Get the view kick
	CBasePlayer *pPlayer = ToBasePlayer(GetOwner());

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
void CWeaponSAW::Operator_HandleAnimEvent(animevent_t *pEvent, CBaseCombatCharacter *pOperator)
{
	switch (pEvent->event)
	{
		DevMsg("event: %d\n", pEvent->event);
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
		ASSERT(npc != NULL);
		vecShootDir = npc->GetActualShootTrajectory(vecShootOrigin);
		//DevMsg("HandleAnimEvent\n");
		FireNPCPrimaryAttack(pOperator, vecShootOrigin, vecShootDir);
	}
	break;


	default:
		CBaseCombatWeapon::Operator_HandleAnimEvent(pEvent, pOperator);
		break;
	}
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CWeaponSAW::FireNPCPrimaryAttack(CBaseCombatCharacter *pOperator, Vector &vecShootOrigin, Vector &vecShootDir)
{
	// FIXME: use the returned number of bullets to account for >10hz firerate
	WeaponSoundRealtime(SINGLE_NPC);

	CSoundEnt::InsertSound(SOUND_COMBAT | SOUND_CONTEXT_GUNFIRE, pOperator->GetAbsOrigin(), SOUNDENT_VOLUME_MACHINEGUN, 0.2, pOperator, SOUNDENT_CHANNEL_WEAPON, pOperator->GetEnemy());
	Vector vColor(0, 0, 255);
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

	pOperator->FireBullets(info);


	pOperator->DoMuzzleFlash();
	m_iClip1 = m_iClip1 - 1;
}