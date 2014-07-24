//========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose:
//
//=============================================================================//

#ifndef WEAPON_WRENCH_H
#define WEAPON_WRENCH_H

#include "basebludgeonweapon.h"

#if defined( _WIN32 )
#pragma once
#endif

#ifdef HL2MP
#error weapon_wrench.h must not be included in hl2mp. The windows compiler will use the wrong class elsewhere if it is.
#endif

#define	WRENCH_RANGE	75.0f
#define	WRENCH_REFIRE	.75f//originally 0.4f

//-----------------------------------------------------------------------------
// CWeaponWrench
//-----------------------------------------------------------------------------

class CWeaponWrench : public CBaseHLBludgeonWeapon
{
public:
	DECLARE_CLASS( CWeaponWrench, CBaseHLBludgeonWeapon );

	DECLARE_SERVERCLASS();
	DECLARE_ACTTABLE();
	DECLARE_DATADESC();
	CWeaponWrench();

	void		Precache( void );

	void		ItemPostFrame( void );

	void		FixIt( void );
	void		FixThink( void );

	void		PrimaryAttack( void );
	void		PrimaryHit( trace_t &traceHit, Activity nHitActivity);

	void		AltAttack( float m_flDmg );
	void		AltHit( trace_t &traceHit, Activity nHitActivity, float m_flDmg );

	void		MeleeViewKick( float m_flMultiplyer );
	
	float		CalcAltDmg( void );

	float		GetRange( void )		{	return	WRENCH_RANGE;	}
	float		GetFireRate( void )		{	return	WRENCH_REFIRE;	}

	void		AddViewKick( void );
	float		GetDamageForActivity( Activity hitActivity );

	virtual int WeaponMeleeAttack1Condition( float flDot, float flDist );

	// Animation event
	virtual void Operator_HandleAnimEvent( animevent_t *pEvent, CBaseCombatCharacter *pOperator );

private:
	// Animation event handlers
	void HandleAnimEventMeleeHit( animevent_t *pEvent, CBaseCombatCharacter *pOperator );

	//CNetworkVar( float, m_flSecondaryAttackTime );
	float m_flPrimaryAttackTime;
	float m_flSecondaryAttackStartTime;
	float m_flSecondaryAttackTime;
	float m_flNextFixTime;
	float m_flFixTime;
};

#endif // WEAPON_WRENCH_H
