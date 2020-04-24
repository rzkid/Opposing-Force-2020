//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: The downtrodden phoenixes of City 17. Timid when unarmed, they will
//			rise up against their Combine oppressors when given a weapon.
//
//=============================================================================//

#ifndef	NPC_PHOENIX_H
#define	NPC_PHOENIX_H

#include "npc_playercompanion.h"

//#include "ai_behavior_functank.h"

struct SquadCandidate_t;

//-----------------------------------------------------------------------------
//
// CLASS: CNPC_Phoenix
//
//-----------------------------------------------------------------------------

//-------------------------------------
// Spawnflags
//-------------------------------------

#define SF_PHOENIX_FOLLOW			( 1 << 16 )	//65536 follow the player as soon as I spawn.
#define	SF_PHOENIX_MEDIC			( 1 << 17 )	//131072
#define SF_PHOENIX_RANDOM_HEAD		( 1 << 18 )	//262144
#define SF_PHOENIX_AMMORESUPPLIER	( 1 << 19 )	//524288
#define SF_PHOENIX_NOT_COMMANDABLE	( 1 << 20 ) //1048576
#define SF_PHOENIX_IGNORE_SEMAPHORE ( 1 << 21 ) //2097152		Work outside the speech semaphore system
#define SF_PHOENIX_USE_RENDER_BOUNDS ( 1 << 22 )//4194304

//-------------------------------------
// Animation events
//-------------------------------------

//-------------------------------------

class CNPC_Phoenix : public CNPC_PlayerCompanion
{
	DECLARE_CLASS( CNPC_Phoenix, CNPC_PlayerCompanion );
public:
	CNPC_Phoenix()
	 :	m_iHead( -1 )
	{
	}

	//---------------------------------
	bool			CreateBehaviors();
	void			Precache();
	void			PrecacheAllOfType();
	void			Spawn();
	void			PostNPCInit();
	virtual void	SelectModel();
	void			Activate();
	virtual void	OnGivenWeapon( CBaseCombatWeapon *pNewWeapon );

	void			OnRestore();
	
	//---------------------------------
	string_t 		GetModelName() const;
	
	Class_T 		Classify();

	bool 			ShouldAlwaysThink();

	//---------------------------------
	// Behavior
	//---------------------------------
	bool			ShouldBehaviorSelectSchedule( CAI_BehaviorBase *pBehavior );
	void 			GatherConditions();
	void			PredictPlayerPush();
	void 			PrescheduleThink();
	void			BuildScheduleTestBits();

	bool			FInViewCone( CBaseEntity *pEntity );

	int				SelectFailSchedule( int failedSchedule, int failedTask, AI_TaskFailureCode_t taskFailCode );
	int				SelectSchedule();

	int 			SelectSchedulePriorityAction();
	int 			SelectScheduleHeal();
	int 			SelectScheduleRetrieveItem();
	int 			SelectScheduleNonCombat();
	int 			SelectScheduleCombat();
	bool			ShouldDeferToFollowBehavior();
	int 			TranslateSchedule( int scheduleType );

	bool			ShouldAcceptGoal( CAI_BehaviorBase *pBehavior, CAI_GoalEntity *pGoal );
	void			OnClearGoal( CAI_BehaviorBase *pBehavior, CAI_GoalEntity *pGoal );
	
	void 			StartTask( const Task_t *pTask );
	void 			RunTask( const Task_t *pTask );
	
	Activity		NPC_TranslateActivity( Activity eNewActivity );
	void 			HandleAnimEvent( animevent_t *pEvent );
	void			TaskFail( AI_TaskFailureCode_t code );

	void 			PickupItem( CBaseEntity *pItem );

	void 			SimpleUse( CBaseEntity *pActivator, CBaseEntity *pCaller, USE_TYPE useType, float value );

	bool			IgnorePlayerPushing( void );

	int				DrawDebugTextOverlays( void );

	virtual const char *SelectRandomExpressionForState( NPC_STATE state );

	//---------------------------------
	// Combat
	//---------------------------------
	bool 			OnBeginMoveAndShoot();
	void 			OnEndMoveAndShoot();
	
	virtual bool	UseAttackSquadSlots()	{ return false; }
	void 			LocateEnemySound();
	
	Vector 			GetActualShootPosition( const Vector &shootOrigin );
	void 			OnChangeActiveWeapon( CBaseCombatWeapon *pOldWeapon, CBaseCombatWeapon *pNewWeapon );

	bool			ShouldLookForBetterWeapon();


	//---------------------------------
	// Damage handling
	//---------------------------------
	int 			OnTakeDamage_Alive( const CTakeDamageInfo &info );
	
	//---------------------------------
	// Commander mode
	//---------------------------------
	bool 			IsCommandable();
	bool			IsPlayerAlly( CBasePlayer *pPlayer = NULL );
	bool			CanJoinPlayerSquad();
	bool			WasInPlayerSquad();
	bool			HaveCommandGoal() const;
	bool			IsCommandMoving();
	bool			ShouldAutoSummon();
	bool 			IsValidCommandTarget( CBaseEntity *pTarget );
	bool 			NearCommandGoal();
	bool 			VeryFarFromCommandGoal();
	bool 			TargetOrder( CBaseEntity *pTarget, CAI_BaseNPC **Allies, int numAllies );
	void 			MoveOrder( const Vector &vecDest, CAI_BaseNPC **Allies, int numAllies );
	void			OnMoveOrder();
	void 			CommanderUse( CBaseEntity *pActivator, CBaseEntity *pCaller, USE_TYPE useType, float value );
	bool			ShouldSpeakRadio( CBaseEntity *pListener );
	void			OnMoveToCommandGoalFailed();
	void			AddToPlayerSquad();
	void			RemoveFromPlayerSquad();
	void 			TogglePlayerSquadState();
	void			UpdatePlayerSquad();
	static int __cdecl PlayerSquadCandidateSortFunc( const SquadCandidate_t *, const SquadCandidate_t * );
	void 			FixupPlayerSquad();
	void 			ClearFollowTarget();
	void 			UpdateFollowCommandPoint();
	bool			IsFollowingCommandPoint();
	CAI_BaseNPC *	GetSquadCommandRepresentative();
	void			SetSquad( CAI_Squad *pSquad );
	void			AddInsignia();
	void			RemoveInsignia();
	bool			SpeakCommandResponse( AIConcept_t concept, const char *modifiers = NULL );
	
	bool			HandleInteraction(int interactionType, void *data, CBaseCombatCharacter* sourceEnt);
	
	//---------------------------------
	// Hints
	//---------------------------------
	bool			FValidateHintType ( CAI_Hint *pHint );

	//---------------------------------
	// Special abilities
	//---------------------------------
	bool 			IsMedic() 			{ return HasSpawnFlags(SF_PHOENIX_MEDIC); }
	bool 			IsAmmoResupplier() 	{ return HasSpawnFlags(SF_PHOENIX_AMMORESUPPLIER); }
	
	bool 			CanHeal();
	bool 			ShouldHealTarget( CBaseEntity *pTarget, bool bActiveUse = false );
#if HL2_EPISODIC
	bool 			ShouldHealTossTarget( CBaseEntity *pTarget, bool bActiveUse = false );
#endif
	void 			Heal();

	bool			ShouldLookForHealthItem();

#if HL2_EPISODIC
	void			TossHealthKit( CBaseCombatCharacter *pThrowAt, const Vector &offset ); // create a healthkit and throw it at someone
	void			InputForceHealthKitToss( inputdata_t &inputdata );
#endif
	
	//---------------------------------
	// Inputs
	//---------------------------------
	void			InputRemoveFromPlayerSquad( inputdata_t &inputdata ) { RemoveFromPlayerSquad(); }
	void 			InputStartPatrolling( inputdata_t &inputdata );
	void 			InputStopPatrolling( inputdata_t &inputdata );
	void			InputSetCommandable( inputdata_t &inputdata );
	void			InputSetMedicOn( inputdata_t &inputdata );
	void			InputSetMedicOff( inputdata_t &inputdata );
	void			InputSetAmmoResupplierOn( inputdata_t &inputdata );
	void			InputSetAmmoResupplierOff( inputdata_t &inputdata );
	void			InputSpeakIdleResponse( inputdata_t &inputdata );

	//---------------------------------
	//	Sounds & speech
	//---------------------------------
	void			FearSound( void );
	void			DeathSound( const CTakeDamageInfo &info );
	bool			UseSemaphore( void );

	virtual void	OnChangeRunningBehavior( CAI_BehaviorBase *pOldBehavior,  CAI_BehaviorBase *pNewBehavior );

private:
	//-----------------------------------------------------
	// Conditions, Schedules, Tasks
	//-----------------------------------------------------
	enum
	{
		COND_PHO_PLAYERHEALREQUEST = BaseClass::NEXT_CONDITION,
		COND_PHO_COMMANDHEAL,
		COND_PHO_HURTBYFIRE,
		
		SCHED_PHOENIX_PLAY_INSPECT_ACTIVITY = BaseClass::NEXT_SCHEDULE,
		SCHED_PHOENIX_HEAL,
		SCHED_PHOENIX_RANGE_ATTACK1_RPG,
		SCHED_PHOENIX_PATROL,
		SCHED_PHOENIX_MOURN_PLAYER,
		SCHED_PHOENIX_STRIDER_RANGE_ATTACK1_RPG,
		
		TASK_PHO_HEAL = BaseClass::NEXT_TASK,
		TASK_PHO_RPG_AUGER,
		TASK_PHO_PLAY_INSPECT_SEQUENCE,
		TASK_PHO_SPEAK_MOURNING,

	};

	//-----------------------------------------------------
	
	int				m_nInspectActivity;
	float			m_flNextFearSoundTime;
	float			m_flPlayerHealTime;
	float			m_flNextHealthSearchTime; // Next time I'm allowed to look for a healthkit
	float			m_flAllyHealTime;
	float			m_flPlayerGiveAmmoTime;
	string_t		m_iszAmmoSupply;
	int				m_iAmmoAmount;
	bool			m_bRPGAvoidPlayer;
	bool			m_bShouldPatrol;
	string_t		m_iszOriginalSquad;
	float			m_flTimeJoinedPlayerSquad;
	bool			m_bWasInPlayerSquad;
	float			m_flTimeLastCloseToPlayer;
	string_t		m_iszDenyCommandConcept;

	CSimpleSimTimer	m_AutoSummonTimer;
	Vector			m_vAutoSummonAnchor;

	int				m_iHead;

	static CSimpleSimTimer gm_PlayerSquadEvaluateTimer;

	float			m_flTimePlayerStare;	// The game time at which the player started staring at me.
	float			m_flTimeNextHealStare;	// Next time I'm allowed to heal a player who is staring at me.

	//-----------------------------------------------------
	//	Outputs
	//-----------------------------------------------------
	COutputEvent		m_OnJoinedPlayerSquad;
	COutputEvent		m_OnLeftPlayerSquad;
	COutputEvent		m_OnFollowOrder;
	COutputEvent		m_OnStationOrder; 
	COutputEvent		m_OnPlayerUse;
	COutputEvent		m_OnNavFailBlocked;

	//-----------------------------------------------------
//	CAI_FuncTankBehavior	m_FuncTankBehavior;

	CHandle<CAI_FollowGoal>	m_hSavedFollowGoalEnt;

	bool					m_bNotifyNavFailBlocked;
	bool					m_bNeverLeavePlayerSquad; // Don't leave the player squad unless killed, or removed via Entity I/O. 
	
	//-----------------------------------------------------
	
	DECLARE_DATADESC();
#ifdef _XBOX
protected:
#endif
	DEFINE_CUSTOM_AI;
};

//---------------------------------------------------------
//---------------------------------------------------------
inline bool CNPC_Phoenix::NearCommandGoal()
{
	const float flDistSqr = COMMAND_GOAL_TOLERANCE * COMMAND_GOAL_TOLERANCE;
	return ( ( GetAbsOrigin() - GetCommandGoal() ).LengthSqr() <= flDistSqr );
}

//---------------------------------------------------------
//---------------------------------------------------------
inline bool CNPC_Phoenix::VeryFarFromCommandGoal()
{
	const float flDistSqr = (12*50) * (12*50);
	return ( ( GetAbsOrigin() - GetCommandGoal() ).LengthSqr() > flDistSqr );
}



//==============================================================================
// PHOENIX PLAYER-RESPONSE SYSTEM
//
// NOTE: This system is obsolete, and left here for legacy support.
//		 It has been superseded by the ai_eventresponse system.
//
//==============================================================================
#define PHOENIX_RESPONSE_DISTANCE			768			// Maximum distance for responding phoenixes
#define PHOENIX_RESPONSE_REFIRE_TIME		15.0		// Time after giving a response before giving any more
#define PHOENIX_RESPONSE_GIVEUP_TIME		4.0			// Time after a response trigger was fired before discarding it without responding

enum phoenixresponses_t
{
	CR_PLAYER_SHOT_GUNSHIP,		// Player has shot the gunship with a bullet weapon
	CR_PLAYER_KILLED_GUNSHIP,	// Player has destroyed the gunship
	CR_VITALNPC_DIED,			// Mapmaker specified that an NPC that was vital has died

	// Add new responses here

	MAX_PHOENIX_RESPONSES,
};

//-------------------------------------

class CPhoenixResponseSystem : public CBaseEntity
{
	DECLARE_CLASS( CPhoenixResponseSystem, CBaseEntity );
public:
	DECLARE_DATADESC();

	void	Spawn();
	void	OnRestore();

	void	AddResponseTrigger( phoenixresponses_t	iTrigger );

	void	ResponseThink();

	//---------------------------------
	// Inputs
	//---------------------------------
	void 	InputResponseVitalNPC( inputdata_t &inputdata );

private:
	float	m_flResponseAddedTime[ MAX_PHOENIX_RESPONSES ];		// Time at which the response was added. 0 if we have no response.
	float	m_flNextResponseTime;
};

//-------------------------------------

class CSquadInsignia : public CBaseAnimating
{
	DECLARE_CLASS( CSquadInsignia, CBaseAnimating );
	void Spawn();
};

//-------------------------------------

CPhoenixResponseSystem	*GetPhoenixResponse();

//-----------------------------------------------------------------------------

#endif	//NPC_PHOENIX_H
