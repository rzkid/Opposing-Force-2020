//========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose:		Wrench - a new favorite
//
// $NoKeywords: $
//=============================================================================//

#include "cbase.h"
#include "basehlcombatweapon.h"
#include "player.h"
#include "gamerules.h"
#include "ammodef.h"
#include "mathlib/mathlib.h"
#include "in_buttons.h"
#include "soundent.h"
#include "basebludgeonweapon.h"
#include "vstdlib/random.h"
#include "npcevent.h"
#include "ai_basenpc.h"
#include "weapon_wrench.h"
#include "rumble_shared.h"
#include "GameStats.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

ConVar    sk_plr_dmg_wrench		( "sk_plr_dmg_wrench","0");
ConVar    sk_npc_dmg_wrench		( "sk_npc_dmg_wrench","0");

ConVar    sk_plr_alt_dmg_wrench_min	( "sk_plr_alt_dmg_wrench_min","30");
ConVar    sk_plr_alt_dmg_wrench_max	( "sk_plr_alt_dmg_wrench_max","75");

ConVar		dev_wrench_fix_refire ( "dev_wrench_fix_refire","1.8");

#define BLUDGEON_HULL_DIM		16

static const Vector g_bludgeonMins(-BLUDGEON_HULL_DIM,-BLUDGEON_HULL_DIM,-BLUDGEON_HULL_DIM);
static const Vector g_bludgeonMaxs(BLUDGEON_HULL_DIM,BLUDGEON_HULL_DIM,BLUDGEON_HULL_DIM);

#define FIX_THINK_CONTEXT "FixThinkContext"

#define FIX_DAMAGE -15

//-----------------------------------------------------------------------------
// CWeaponWrench
//-----------------------------------------------------------------------------

IMPLEMENT_SERVERCLASS_ST(CWeaponWrench, DT_WeaponWrench)
END_SEND_TABLE()

#ifndef HL2MP
LINK_ENTITY_TO_CLASS( weapon_wrench, CWeaponWrench );
PRECACHE_WEAPON_REGISTER( weapon_wrench );
#endif

acttable_t CWeaponWrench::m_acttable[] = 
{
	{ ACT_MELEE_ATTACK1,	ACT_MELEE_ATTACK_SWING, true },
	{ ACT_IDLE,				ACT_IDLE_ANGRY_MELEE,	false },
	{ ACT_IDLE_ANGRY,		ACT_IDLE_ANGRY_MELEE,	false },
};

IMPLEMENT_ACTTABLE(CWeaponWrench);

BEGIN_DATADESC(CWeaponWrench)
	DEFINE_FUNCTION( FixThink ),
END_DATADESC()

//-----------------------------------------------------------------------------
// Constructor
//-----------------------------------------------------------------------------
CWeaponWrench::CWeaponWrench( void )
{
	m_flPrimaryAttackTime = 0;
	m_flSecondaryAttackStartTime = 0;
	m_flSecondaryAttackTime = 0;
	m_flNextFixTime = 0.0f;
}

void CWeaponWrench::Precache( void )
{
	PrecacheScriptSound("Weapon_Wrench.Strike");
	BaseClass::Precache();
}

void CWeaponWrench::ItemPostFrame( void )
{
	CBasePlayer *pOwner = ToBasePlayer( GetOwner() );
	
	if ( pOwner == NULL )
		return;

	if ( (pOwner->m_nButtons & IN_ATTACK2) /*&& (m_flNextSecondaryAttack <= gpGlobals->curtime)*/ )
	{
		FixIt();
		if(m_flNextSecondaryAttack <= gpGlobals->curtime)
		{
			//button was just pressed, attack started
			if ( m_flSecondaryAttackStartTime <= 0.0f )
			{
				//SecondaryAttack(false);
				Msg("ATTACK STARTED \n");
				m_flSecondaryAttackStartTime = gpGlobals->curtime;
				SendWeaponAnim( ACT_VM_HAULBACK );
				//m_flNextSecondaryAttack = gpGlobals->curtime + SequenceDuration();
			}

			//button still pressed, continue attack
			else
			{
				Msg("ATTACK ON HOLD, damage to give: %f \n", CalcAltDmg() );

				//SendWeaponAnim( ACT_VM_DRAW );
			}
		}
	}
	else if ( (pOwner->m_nButtons & IN_ATTACK)  )
	{
		//DevMsg("primary attack\n");
		//PrimaryAttack();
		FixIt();
		if (m_flPrimaryAttackTime <= 0 && (m_flNextPrimaryAttack <= gpGlobals->curtime))
		{
			m_flPrimaryAttackTime = gpGlobals->curtime + 0.4;
			trace_t traceHit;
			Vector swingStart = pOwner->Weapon_ShootPosition( );
			Vector forward;
			forward = pOwner->GetAutoaimVector( AUTOAIM_SCALE_DEFAULT, GetRange() );
			Vector swingEnd = swingStart + forward * GetRange();
			UTIL_TraceLine( swingStart, swingEnd, MASK_SHOT_HULL, pOwner, COLLISION_GROUP_NONE, &traceHit );
			//CBaseEntity	*pHitEntity = traceHit.m_pEnt;
			Activity nHitActivity;

			nHitActivity = ACT_VM_HITCENTER;
			SendWeaponAnim( nHitActivity );
			
		}
		else if (gpGlobals->curtime >= (m_flPrimaryAttackTime) && m_flPrimaryAttackTime > 0)
		{
			WeaponSound( SINGLE );
			//DevMsg("hit!\n");
			PrimaryAttack();
			m_flPrimaryAttackTime = -1;

		}
	}
	else 
	{
		
		
		if ( m_flSecondaryAttackStartTime > 0)
		{
			//button was released
			//DevMsg("button was released and m_flSecondaryAttackTime is %f\n",m_flSecondaryAttackStartTime);
			//SecondaryAttack(true);

			Msg("ATTACK ENDED \n");
		
			//SendWeaponAnim( ACT_VM_SWINGHARD );
			//float damage = CalcAltDmg();
			trace_t traceHit;
			Vector swingStart = pOwner->Weapon_ShootPosition( );
			Vector forward;
			forward = pOwner->GetAutoaimVector( AUTOAIM_SCALE_DEFAULT, GetRange() );
			Vector swingEnd = swingStart + forward * GetRange();
			UTIL_TraceLine( swingStart, swingEnd, MASK_SHOT_HULL, pOwner, COLLISION_GROUP_NONE, &traceHit );
			//CBaseEntity	*pHitEntity = traceHit.m_pEnt;
			Activity nHitActivity;
			nHitActivity = ACT_VM_SWINGHARD;//ACT_VM_HITCENTER;

			SendWeaponAnim( nHitActivity );

			//AltAttack( CalcAltDmg() );
			//DevMsg("CalcAltDmg() is %f\n",damage);
			m_flSecondaryAttackStartTime = 0;
			//DevMsg("the sequence duration is %f\n",SequenceDuration());
			m_flSecondaryAttackTime = gpGlobals->curtime + .35f;
			//m_flNextSecondaryAttack = gpGlobals->curtime + SequenceDuration() + 0.6f;
		}
		else if (gpGlobals->curtime > m_flSecondaryAttackTime && m_flSecondaryAttackTime > 0)
		{
			AltAttack( CalcAltDmg() );
			m_flNextSecondaryAttack = gpGlobals->curtime + SequenceDuration() + 0.6f;
			m_flNextPrimaryAttack = gpGlobals->curtime + .55f;
			m_flSecondaryAttackTime = -1;
			m_flPrimaryAttackTime = -1;
		}
		else if (gpGlobals->curtime > (m_flPrimaryAttackTime) && m_flPrimaryAttackTime > 0)
		{
			WeaponSound( SINGLE );
			//DevMsg("hit!\n");
			PrimaryAttack();
			m_flPrimaryAttackTime = -1;

		}
		else
		{
			//DevMsg("idle\n");
			WeaponIdle();
			return;
		}
	}
}

//
//	sets the anim, sounds, and attack times
//
//
void CWeaponWrench::FixIt( void )
{
	CBasePlayer *pOwner = ToBasePlayer( GetOwner() );
	trace_t traceHit;
	Vector swingStart = pOwner->Weapon_ShootPosition( );
	Vector forward;
	forward = pOwner->GetAutoaimVector( AUTOAIM_SCALE_DEFAULT, GetRange() );
	Vector swingEnd = swingStart + forward * GetRange();
	UTIL_TraceLine( swingStart, swingEnd, MASK_SHOT_HULL, pOwner, COLLISION_GROUP_NONE, &traceHit );
	CBaseEntity	*pHitEntity = traceHit.m_pEnt;
	//Activity nHitActivity;
	if (pHitEntity != NULL)
	{	
		if (FClassnameIs(pHitEntity, "prop_vehicle_gauss_cannon" ) == false &&
			FClassnameIs(pHitEntity, "prop_fixable" ) == false)
		{
			return;
		}
		else
		{
			
			m_flNextSecondaryAttack = gpGlobals->curtime + 0.4f;
			m_flNextPrimaryAttack = gpGlobals->curtime + 0.4f;

			if ( m_flNextFixTime >= gpGlobals->curtime )
				return;

			DevMsg("FIX IT!!!!\n");
			WeaponSound( SPECIAL1 );
			m_flNextFixTime = gpGlobals->curtime + dev_wrench_fix_refire.GetFloat();

			SendWeaponAnim(ACT_VM_FIX_GAUSS);

			//	maybe use a context think for the actual fixing?
			SetContextThink( &CWeaponWrench::FixThink, gpGlobals->curtime + 1.0f, FIX_THINK_CONTEXT );
			
			////DevMsg("fix the gauss cannon!!\n");
			//if( FClassnameIs(pHitEntity, "prop_vehicle_gauss_cannon" ) == true  )
			//{
			//	info.SetDamage(-200);
			//	isGauss = true;
			//	DevMsg("primary damage: %f\n",info.GetDamage());
			//}
		}
	}
	else
		return;

}

void CWeaponWrench::FixThink( void )
{
	DevMsg("Bazinga!\n");

	trace_t traceHit;

	// Try a ray
	CBasePlayer *pOwner = ToBasePlayer( GetOwner() );
	if ( !pOwner )
		return;

	//Owner->RumbleEffect( RUMBLE_CROWBAR_SWING, 0, RUMBLE_FLAG_RESTART );

	Vector swingStart = pOwner->Weapon_ShootPosition( );
	Vector forward;

	forward = pOwner->GetAutoaimVector( AUTOAIM_SCALE_DEFAULT, GetRange() );

	Vector swingEnd = swingStart + forward * GetRange();
	UTIL_TraceLine( swingStart, swingEnd, MASK_SHOT_HULL, pOwner, COLLISION_GROUP_NONE, &traceHit );
	
	CBaseEntity	*pHitEntity = traceHit.m_pEnt;
	//Activity nHitActivity;
	if (pHitEntity != NULL) 
	{
		if (FClassnameIs(pHitEntity, "prop_vehicle_gauss_cannon" ) == false &&
			FClassnameIs(pHitEntity, "prop_fixable" ) == false)
		{
			return;
		}
		//nHitActivity = ACT_VM_HITCENTER;
	}
	else
	{
		return;
	}

	// Like bullets, bludgeon traces have to trace against triggers.
	CTakeDamageInfo triggerInfo( GetOwner(), GetOwner(), FIX_DAMAGE, DMG_CLUB );
	triggerInfo.SetDamagePosition( traceHit.startpos );
	triggerInfo.SetDamageForce( forward );
	TraceAttackToTriggers( triggerInfo, traceHit.startpos, traceHit.endpos, forward );


	Vector hitDirection;
	pOwner->EyeVectors( &hitDirection, NULL, NULL );
	VectorNormalize( hitDirection );

	CTakeDamageInfo info( GetOwner(), GetOwner(), FIX_DAMAGE, DMG_CLUB );
		
	CalculateMeleeDamageForce( &info, hitDirection, traceHit.endpos );

	pHitEntity->DispatchTraceAttack( info, hitDirection, &traceHit ); 
	ApplyMultiDamage();

	// Now hit all triggers along the ray that... 
	TraceAttackToTriggers( info, traceHit.startpos, traceHit.endpos, hitDirection );
}

//------------------------------------------------------------------------------
// Purpose : Starts the swing of the weapon and determines the animation
// Input   : bIsSecondary - is this a secondary attack?
//------------------------------------------------------------------------------
void CWeaponWrench::PrimaryAttack( void )
{
	trace_t traceHit;

	// Try a ray
	CBasePlayer *pOwner = ToBasePlayer( GetOwner() );
	if ( !pOwner )
		return;

	pOwner->RumbleEffect( RUMBLE_CROWBAR_SWING, 0, RUMBLE_FLAG_RESTART );

	Vector swingStart = pOwner->Weapon_ShootPosition( );
	Vector forward;

	forward = pOwner->GetAutoaimVector( AUTOAIM_SCALE_DEFAULT, GetRange() );

	Vector swingEnd = swingStart + forward * GetRange();
	UTIL_TraceLine( swingStart, swingEnd, MASK_SHOT_HULL, pOwner, COLLISION_GROUP_NONE, &traceHit );
	
//	CBaseEntity	*pHitEntity = traceHit.m_pEnt;
	Activity nHitActivity;
	//if (pHitEntity != NULL) 
	//{
		nHitActivity = ACT_VM_HITCENTER;
	//}

	// Like bullets, bludgeon traces have to trace against triggers.
	CTakeDamageInfo triggerInfo( GetOwner(), GetOwner(), GetDamageForActivity( nHitActivity ), DMG_CLUB );
	triggerInfo.SetDamagePosition( traceHit.startpos );
	triggerInfo.SetDamageForce( forward );
	TraceAttackToTriggers( triggerInfo, traceHit.startpos, traceHit.endpos, forward );

	if ( traceHit.fraction == 1.0 )
	{
		float bludgeonHullRadius = 1.732f * BLUDGEON_HULL_DIM;  // hull is +/- 16, so use cuberoot of 2 to determine how big the hull is from center to the corner point

		// Back off by hull "radius"
		swingEnd -= forward * bludgeonHullRadius;

		UTIL_TraceHull( swingStart, swingEnd, g_bludgeonMins, g_bludgeonMaxs, MASK_SHOT_HULL, pOwner, COLLISION_GROUP_NONE, &traceHit );
		if ( traceHit.fraction < 1.0 && traceHit.m_pEnt )
		{
			Vector vecToTarget = traceHit.m_pEnt->GetAbsOrigin() - swingStart;
			VectorNormalize( vecToTarget );

			float dot = vecToTarget.Dot( forward );

			// YWB:  Make sure they are sort of facing the guy at least...
			if ( dot < 0.70721f )
			{
				// Force amiss
				traceHit.fraction = 1.0f;
			}
			else
			{
				nHitActivity = ChooseIntersectionPointAndActivity( traceHit, g_bludgeonMins, g_bludgeonMaxs, pOwner );
			}
		}
	}


		m_iPrimaryAttacks++;


	gamestats->Event_WeaponFired( pOwner, true, GetClassname() );

	IGameEvent* event = gameeventmanager->CreateEvent("use_primaryattack");

	if (event)
	{
		event->SetInt("userid", pOwner->GetUserID());
		gameeventmanager->FireEvent(event);
	}

	// -------------------------
	//	Miss
	// -------------------------
	if ( traceHit.fraction == 1.0f )
	{
		nHitActivity = ACT_VM_MISSCENTER;

		// We want to test the first swing again
		Vector testEnd = swingStart + forward * GetRange();
		
		// See if we happened to hit water
		ImpactWater( swingStart, testEnd );
	}
	else
	{
		PrimaryHit( traceHit, nHitActivity);
	}

	// Send the anim
	//SendWeaponAnim( nHitActivity );

	//Setup our next attack times
	m_flNextPrimaryAttack = gpGlobals->curtime + .55f;//+ SequenceDuration();//GetFireRate();
	//DevMsg("primary attack\n");
	//m_flNextSecondaryAttack = gpGlobals->curtime + SequenceDuration();

	//Play swing sound
	//WeaponSound( SINGLE );
}

//------------------------------------------------------------------------------
// Purpose: Implement impact function
//------------------------------------------------------------------------------
void CWeaponWrench::PrimaryHit( trace_t &traceHit, Activity nHitActivity)
{
	CBasePlayer *pPlayer = ToBasePlayer( GetOwner() );
	
	//Do view kick
	AddViewKick();

	//Make sound for the AI
	CSoundEnt::InsertSound( SOUND_BULLET_IMPACT, traceHit.endpos, 400, 0.2f, pPlayer );
	EmitSound("Weapon_Wrench.Strike");
	//DevMsg("Weapon_Wrench.Strike\n");

	// This isn't great, but it's something for when the crowbar hits.
	pPlayer->RumbleEffect( RUMBLE_AR2, 0, RUMBLE_FLAG_RESTART );

	CBaseEntity	*pHitEntity = traceHit.m_pEnt;
	
	bool isGauss = false;
	//Apply damage to a hit target
	if ( pHitEntity != NULL )
	{
		Vector hitDirection;
		pPlayer->EyeVectors( &hitDirection, NULL, NULL );
		VectorNormalize( hitDirection );

		CTakeDamageInfo info( GetOwner(), GetOwner(), GetDamageForActivity( nHitActivity ), DMG_CLUB );
		
		

		
		if( pPlayer && pHitEntity->IsNPC() )
		{
			// If bonking an NPC, adjust damage.
			info.AdjustPlayerDamageInflictedForSkillLevel();
		}

		CalculateMeleeDamageForce( &info, hitDirection, traceHit.endpos );

		pHitEntity->DispatchTraceAttack( info, hitDirection, &traceHit ); 
		ApplyMultiDamage();

		// Now hit all triggers along the ray that... 
		TraceAttackToTriggers( info, traceHit.startpos, traceHit.endpos, hitDirection );

		if ( ToBaseCombatCharacter( pHitEntity ) )
		{
			gamestats->Event_WeaponHit( pPlayer, true, GetClassname(), info );
		}
	}

	// Apply an impact effect
	if(!isGauss) ImpactEffect( traceHit );
}

void CWeaponWrench::AltAttack( float m_flDmg )
{
	//DevMsg("in CWeaponWrench::MeleeAttack float m_flDmg is %f\n",m_flDmg);
	trace_t traceHit;

	// Try a ray
	CBasePlayer *pOwner = ToBasePlayer( GetOwner() );
	if ( !pOwner )
		return;

	Vector swingStart = pOwner->Weapon_ShootPosition( );
	Vector forward;

	forward = pOwner->GetAutoaimVector( AUTOAIM_SCALE_DEFAULT, GetRange() );

	Vector swingEnd = swingStart + forward * GetRange();
	UTIL_TraceLine( swingStart, swingEnd, MASK_SHOT_HULL, pOwner, COLLISION_GROUP_NONE, &traceHit );

	//CBaseEntity	*pHitEntity = traceHit.m_pEnt;
	Activity nHitActivity;
	//if (pHitEntity != NULL )
	//{
		nHitActivity = ACT_VM_SWINGHARD;//ACT_VM_HITCENTER;
	//}

	// Like bullets, bludgeon traces have to trace against triggers.
	CTakeDamageInfo triggerInfo( GetOwner(), GetOwner(), GetDamageForActivity( nHitActivity ), DMG_CLUB );
	triggerInfo.SetDamagePosition( traceHit.startpos );
	triggerInfo.SetDamageForce( forward );
	TraceAttackToTriggers( triggerInfo, traceHit.startpos, traceHit.endpos, forward );

	if ( traceHit.fraction == 1.0 )
	{
		float bludgeonHullRadius = 1.732f * /*MELEE_HULL_DIM*/BLUDGEON_HULL_DIM;  // hull is +/- 16, so use cuberoot of 2 to determine how big the hull is from center to the corner point

		// Back off by hull "radius"
		swingEnd -= forward * bludgeonHullRadius;

		if ( traceHit.fraction < 1.0 && traceHit.m_pEnt )
		{
			Vector vecToTarget = traceHit.m_pEnt->GetAbsOrigin() - swingStart;
			VectorNormalize( vecToTarget );

			float dot = vecToTarget.Dot( forward );

			// YWB:  Make sure they are sort of facing the guy at least...
			if ( dot < 0.70721f )
			{
				// Force amiss
				traceHit.fraction = 1.0f;
			}
			else
			{
				//nHitActivity = ChooseIntersectionPointAndActivity( traceHit, g_bludgeonMins, g_bludgeonMaxs, pOwner );
			}
		}
	}

	// -------------------------
	//	Miss
	// -------------------------
	if ( traceHit.fraction == 1.0f )
	{
		// We want to test the first swing again
		Vector testEnd = swingStart + forward * GetRange();
		
		// See if we happened to hit water
		BaseClass::ImpactWater( swingStart, testEnd );
	}
	else
	{
		AltHit( traceHit, nHitActivity, m_flDmg );
	}

	// Send the anim
	//SendWeaponAnim( nHitActivity );

	//Play swing sound
	WeaponSound( SINGLE );
}

//------------------------------------------------------------------------------
// Purpose: Implement impact function
//------------------------------------------------------------------------------
void CWeaponWrench::AltHit( trace_t &traceHit, Activity nHitActivity, float m_flDmg )
{
	//DevMsg("Hello\n");
	//DevMsg("in CWeaponWrench::WrenchHit float m_flDmg is %f\n",m_flDmg);
	CBasePlayer *pPlayer = ToBasePlayer( GetOwner() );
	
	//Do view kick
	MeleeViewKick(m_flDmg);

	//Make sound for the AI
	CSoundEnt::InsertSound( SOUND_BULLET_IMPACT, traceHit.endpos, 400, 0.2f, pPlayer );
	EmitSound("Weapon_Wrench.Strike");
	//DevMsg("Weapon_Wrench.Strike\n");

	// This isn't great, but it's something for when the crowbar hits.
	pPlayer->RumbleEffect( RUMBLE_AR2, 0, RUMBLE_FLAG_RESTART );

	CBaseEntity	*pHitEntity = traceHit.m_pEnt;

	bool isGauss = false;
	//Apply damage to a hit target
	if ( pHitEntity != NULL )
	{
		Vector hitDirection;
		pPlayer->EyeVectors( &hitDirection, NULL, NULL );
		VectorNormalize( hitDirection );

		CTakeDamageInfo info( GetOwner(), GetOwner(), m_flDmg, DMG_CLUB );

		//DevMsg("fix the gauss cannon!!\n");
		if( FClassnameIs(pHitEntity, "prop_vehicle_gauss_cannon" ) == true  )
		{
			//DevMsg("fix Gauss Cannon!\n");
			info.SetDamage(info.GetDamage()*-4.5f);
			isGauss = true;
			DevMsg("alt damage: %f\n",info.GetDamage());
		}
		/*if( pPlayer && pHitEntity->IsNPC() )
		{
			// If bonking an NPC, adjust damage.
			info.AdjustPlayerDamageInflictedForSkillLevel();
		}*/

		CalculateMeleeDamageForce( &info, hitDirection, traceHit.endpos );

		pHitEntity->DispatchTraceAttack( info, hitDirection, &traceHit ); 
		ApplyMultiDamage();

		// Now hit all triggers along the ray that... 
		TraceAttackToTriggers( info, traceHit.startpos, traceHit.endpos, hitDirection );
	}

	// Apply an impact effect
	if(!isGauss) ImpactEffect( traceHit );
}

//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
void CWeaponWrench::MeleeViewKick( float m_flMultiplyer )
{
	//DevMsg("in CWeaponWrench::MeleeViewKick\n");
	CBasePlayer *pPlayer  = ToBasePlayer( GetOwner() );
	
	if ( pPlayer == NULL )
		return;

	QAngle punchAng;

	punchAng.x = m_flMultiplyer/100;
	punchAng.y = random->RandomFloat( -2.0f, -1.0f );

	punchAng.z = 0.0f;
	
	pPlayer->ViewPunch( punchAng ); 
}

float CWeaponWrench::CalcAltDmg( void )
{
	//DevMsg("in CWeaponWrench::CalcAltDmg\n");
	float m_fDmg = (gpGlobals->curtime - m_flSecondaryAttackStartTime) * sk_plr_alt_dmg_wrench_min.GetFloat();
	//DevMsg("(gpGlobals->curtime - m_flSecondaryAttackTime) is now %f\n",gpGlobals->curtime - m_flSecondaryAttackStartTime);
	//DevMsg("m_flSecondaryAttackTime is now %f\n",m_flSecondaryAttackStartTime);
	//DevMsg("gpGlobals->curtime is now %f\n",gpGlobals->curtime);
	if ( m_fDmg > sk_plr_alt_dmg_wrench_max.GetFloat() )
	{
		//DevMsg("the damage is now %f\n",m_fDmg);
		return sk_plr_alt_dmg_wrench_max.GetFloat();
	}
	//DevMsg("the damage is now %f\n",m_fDmg);
	return m_fDmg;
}

//-----------------------------------------------------------------------------
// Purpose: Add in a view kick for this weapon
//-----------------------------------------------------------------------------
void CWeaponWrench::AddViewKick( void )
{
	//DevMsg("in CWeaponWrench::AddViewKick\n");
	CBasePlayer *pPlayer  = ToBasePlayer( GetOwner() );
	
	if ( pPlayer == NULL )
		return;

	QAngle punchAng;

	punchAng.x = random->RandomFloat( 1.0f, 2.0f );
	punchAng.y = random->RandomFloat( -2.0f, -1.0f );
	punchAng.z = 0.0f;
	
	pPlayer->ViewPunch( punchAng ); 
}


//-----------------------------------------------------------------------------
// Purpose: Get the damage amount for the animation we're doing
// Input  : hitActivity - currently played activity
// Output : Damage amount
//-----------------------------------------------------------------------------
float CWeaponWrench::GetDamageForActivity( Activity hitActivity )
{
	//DevMsg("in CWeaponWrench::GetDamageForActivity\n");
	if ( ( GetOwner() != NULL ) && ( GetOwner()->IsPlayer() ) )
		return sk_plr_dmg_wrench.GetFloat();

	return sk_npc_dmg_wrench.GetFloat();
}



//-----------------------------------------------------------------------------
// Attempt to lead the target (needed because citizens can't hit manhacks with the wrench!)
//-----------------------------------------------------------------------------
ConVar sk_wrench_lead_time( "sk_wrench_lead_time", "0.9" );

int CWeaponWrench::WeaponMeleeAttack1Condition( float flDot, float flDist )
{
	//DevMsg("in CWeaponWrench::WeaponMeleeAttack1Condition\n");
	// Attempt to lead the target (needed because citizens can't hit manhacks with the wrench!)
	CAI_BaseNPC *pNPC	= GetOwner()->MyNPCPointer();
	CBaseEntity *pEnemy = pNPC->GetEnemy();
	if (!pEnemy)
		return COND_NONE;

	Vector vecVelocity;
	vecVelocity = pEnemy->GetSmoothedVelocity( );

	// Project where the enemy will be in a little while
	float dt = sk_wrench_lead_time.GetFloat();
	dt += random->RandomFloat( -0.3f, 0.2f );
	if ( dt < 0.0f )
		dt = 0.0f;

	Vector vecExtrapolatedPos;
	VectorMA( pEnemy->WorldSpaceCenter(), dt, vecVelocity, vecExtrapolatedPos );

	Vector vecDelta;
	VectorSubtract( vecExtrapolatedPos, pNPC->WorldSpaceCenter(), vecDelta );

	if ( fabs( vecDelta.z ) > 70 )
	{
		return COND_TOO_FAR_TO_ATTACK;
	}

	Vector vecForward = pNPC->BodyDirection2D( );
	vecDelta.z = 0.0f;
	float flExtrapolatedDist = Vector2DNormalize( vecDelta.AsVector2D() );
	if ((flDist > 64) && (flExtrapolatedDist > 64))
	{
		return COND_TOO_FAR_TO_ATTACK;
	}

	float flExtrapolatedDot = DotProduct2D( vecDelta.AsVector2D(), vecForward.AsVector2D() );
	if ((flDot < 0.7) && (flExtrapolatedDot < 0.7))
	{
		return COND_NOT_FACING_ATTACK;
	}

	return COND_CAN_MELEE_ATTACK1;
}

//-----------------------------------------------------------------------------
// Animation event
//-----------------------------------------------------------------------------
void CWeaponWrench::Operator_HandleAnimEvent( animevent_t *pEvent, CBaseCombatCharacter *pOperator )
{
	//DevMsg("in CWeaponWrench::Operator_HandleAnimEvent\n");
	switch( pEvent->event )
	{
	case EVENT_WEAPON_MELEE_HIT:
		HandleAnimEventMeleeHit( pEvent, pOperator );
		break;

	default:
		BaseClass::Operator_HandleAnimEvent( pEvent, pOperator );
		break;
	}
}

//-----------------------------------------------------------------------------
// Animation event handlers
//-----------------------------------------------------------------------------
void CWeaponWrench::HandleAnimEventMeleeHit( animevent_t *pEvent, CBaseCombatCharacter *pOperator )
{
	//DevMsg("in CWeaponWrench::HandleAnimEventMeleeHit\n");
	// Trace up or down based on where the enemy is...
	// But only if we're basically facing that direction
	Vector vecDirection;
	AngleVectors( GetAbsAngles(), &vecDirection );

	CBaseEntity *pEnemy = pOperator->MyNPCPointer() ? pOperator->MyNPCPointer()->GetEnemy() : NULL;
	if ( pEnemy )
	{
		Vector vecDelta;
		VectorSubtract( pEnemy->WorldSpaceCenter(), pOperator->Weapon_ShootPosition(), vecDelta );
		VectorNormalize( vecDelta );
		
		Vector2D vecDelta2D = vecDelta.AsVector2D();
		Vector2DNormalize( vecDelta2D );
		if ( DotProduct2D( vecDelta2D, vecDirection.AsVector2D() ) > 0.8f )
		{
			vecDirection = vecDelta;
		}
	}

	Vector vecEnd;
	VectorMA( pOperator->Weapon_ShootPosition(), 50, vecDirection, vecEnd );
	CBaseEntity *pHurt = pOperator->CheckTraceHullAttack( pOperator->Weapon_ShootPosition(), vecEnd, 
		Vector(-16,-16,-16), Vector(36,36,36), sk_npc_dmg_wrench.GetFloat(), DMG_CLUB, 0.75 );
	
	// did I hit someone?
	if ( pHurt )
	{
		// play sound
		WeaponSound( MELEE_HIT );
		//DevMsg("play melee_hit sound\n");
		// Fake a trace impact, so the effects work out like a player's crowbar
		trace_t traceHit;
		UTIL_TraceLine( pOperator->Weapon_ShootPosition(), pHurt->GetAbsOrigin(), MASK_SHOT_HULL, pOperator, COLLISION_GROUP_NONE, &traceHit );
		ImpactEffect( traceHit );
	}
	else
	{
		WeaponSound( MELEE_MISS );
		//DevMsg("play melee_miss sound\n");
	}
}