//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose:		Grenade shot from the M16
//
// $Workfile:     $
// $Date:         $
//
//-----------------------------------------------------------------------------
// $Log: $
//
// $NoKeywords: $
//=============================================================================//

#ifndef	GRENADE_M16_H
#define	GRENADE_M16_H

#include "basegrenade_shared.h"

#define	MAX_M16_NO_COLLIDE_TIME 0.2

class SmokeTrail;
class CWeaponAR2;

class CGrenadeM16 : public CBaseGrenade
{
public:
	DECLARE_CLASS(CGrenadeM16, CBaseGrenade);

	CHandle< SmokeTrail > m_hSmokeTrail;
	float				 m_fSpawnTime;
	float				m_fDangerRadius;


	void		Spawn( void );
	void		Precache( void );
	void 		GrenadeM16Touch( CBaseEntity *pOther );
	void		GrenadeM16Think( void );
	void		Event_Killed( const CTakeDamageInfo &info );

public:
	void EXPORT				Detonate(void);
	CGrenadeM16(void);

	DECLARE_DATADESC();
};

#endif	//GRENADE_M16_H
