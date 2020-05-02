/*//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
// $NoKeywords: $
//=============================================================================//
#include "cbase.h"
#include "c_ai_basenpc.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
class C_NPC_Houndeye : public C_AI_BaseNPC
{
public:
	DECLARE_CLASS(C_NPC_Houndeye, C_AI_BaseNPC);
	DECLARE_CLIENTCLASS();

	C_NPC_Houndeye();
	virtual			~C_NPC_Houndeye();

private:
	C_NPC_Houndeye(const C_NPC_Houndeye &);
};

IMPLEMENT_CLIENTCLASS_DT(C_NPC_Houndeye, DT_NPC_Houndeye, CNPC_Houndeye)
END_RECV_TABLE()

C_NPC_Houndeye::C_NPC_Houndeye()
{
	m_bloodColor = BLOOD_COLOR_GREEN;
}

C_NPC_Houndeye::~C_NPC_Houndeye()
{
}
*/