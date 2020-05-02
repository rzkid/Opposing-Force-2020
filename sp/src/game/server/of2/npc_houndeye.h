//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose:		
//
// $Workfile:     $
// $Date:         $
//
//-----------------------------------------------------------------------------
// $Log: $
//
// $NoKeywords: $
//=============================================================================//

#ifndef NPC_HOUNDEYE_H
#define NPC_HOUNDEYE_H
#pragma once


#include	"ai_basenpc.h"

#include	"energy_wave.h"

class CNPC_Houndeye : public CAI_BaseNPC
{
	DECLARE_CLASS(CNPC_Houndeye, CAI_BaseNPC);

public:
	void			Spawn(void);
	void			Precache(void);
	Class_T			Classify(void);
	void			HandleAnimEvent(animevent_t *pEvent);
	float			MaxYawSpeed(void);
	void			WarmUpSound(void);
	void			AlertSound(void);
	void			DeathSound(const CTakeDamageInfo &info);
	void			WarnSound(void);
	void			PainSound(const CTakeDamageInfo &info);
	void			IdleSound(void);
	void			StartTask(const Task_t *pTask);
	void			RunTask(const Task_t *pTask);
	int				GetSoundInterests(void);
	void			SonicAttack(void);
	void			PrescheduleThink(void);
	Vector			WriteBeamColor(void);
	int				RangeAttack1Conditions(float flDot, float flDist);
	bool			FCanActiveIdle(void);
	virtual int		TranslateSchedule(int scheduleType);
	Activity		NPC_TranslateActivity(Activity eNewActivity);
	virtual int		SelectSchedule(void);
	bool			HandleInteraction(int interactionType, void *data, CBaseCombatCharacter* sourceEnt);
	void			NPCThink(void);
	int				OnTakeDamage_Alive(const CTakeDamageInfo &info);
	void			Event_Killed(const CTakeDamageInfo &info);
	bool			IsAnyoneInSquadAttacking(void);
	void			SpeakSentence(int sentenceType);

	float			m_flNextSecondaryAttack;
	bool			m_bLoopClockwise;

	CEnergyWave*	m_pEnergyWave;
	float			m_flEndEnergyWaveTime;

//	float MaxYawSpeed(void);

//	Class_T	Classify(void);

//	void HandleAnimEvent(animevent_t *pEvent);

//	void SonicAttack(void);


//	bool ShouldGoToIdleState(void);
//	bool FValidateHintType(CAI_Hint *pHint);

//	void SetActivity(Activity NewActivity);

//	void StartTask(const Task_t *pTask);
//	void RunTask(const Task_t *pTask);
//	void PrescheduleThink(void);

//	int TranslateSchedule(int scheduleType);
//	int SelectSchedule(void);

	float FLSoundVolume(CSound *pSound);
//	int RangeAttack1Conditions(float flDot, float flDist);

	DEFINE_CUSTOM_AI;
	DECLARE_DATADESC();

private:

	int m_iSpriteTexture;
	bool m_fAsleep;// some houndeyes sleep in idle mode if this is set, the houndeye is lying down
	bool m_fDontBlink;// don't try to open/close eye if this bit is set!
	Vector	m_vecPackCenter; // the center of the pack. The leader maintains this by averaging the origins of all pack members.
};


#endif // NPC_HOUNDEYE_H
