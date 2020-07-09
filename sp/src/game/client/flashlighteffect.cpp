//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
//===========================================================================//

#include "cbase.h"
#include "flashlighteffect.h"
#include "dlight.h"
#include "iefx.h"
#include "iviewrender.h"
#include "view.h"
#include "engine/ivdebugoverlay.h"
#include "tier0/vprof.h"
#include "tier1/KeyValues.h"
#include "toolframework_client.h"

extern ConVar r_flashlightdepthres;

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

extern ConVar r_flashlightdepthtexture;

void r_newnightvisionCallback_f( IConVar *pConVar, const char *pOldString, float flOldValue );

static ConVar r_newnightvision( "r_newnightvision", "1", FCVAR_CHEAT, "", r_newnightvisionCallback_f );
static ConVar r_swingnightvision( "r_swingnightvision", "1", FCVAR_CHEAT );
static ConVar r_nightvisionlockposition( "r_nightvisionlockposition", "0", FCVAR_CHEAT );
//static ConVar r_nightvisionfov( "r_nightvisionfov", "45.0", FCVAR_CHEAT );
static ConVar r_nightvisionoffsetx( "r_nightvisionoffsetx", "10.0", FCVAR_CHEAT );
static ConVar r_nightvisionoffsety( "r_nightvisionoffsety", "-20.0", FCVAR_CHEAT );
static ConVar r_nightvisionoffsetz( "r_nightvisionoffsetz", "24.0", FCVAR_CHEAT );
static ConVar r_nightvisionnear( "r_nightvisionnear", "4.0", FCVAR_CHEAT );
static ConVar r_nightvisionfar( "r_nightvisionfar", "750.0", FCVAR_CHEAT );
static ConVar r_nightvisionconstant( "r_nightvisionconstant", "0.0", FCVAR_CHEAT );
static ConVar r_nightvisionlinear( "r_nightvisionlinear", "100.0", FCVAR_CHEAT );
static ConVar r_nightvisionquadratic( "r_nightvisionquadratic", "0.0", FCVAR_CHEAT );
static ConVar r_nightvisionvisualizetrace( "r_nightvisionvisualizetrace", "0", FCVAR_CHEAT );
static ConVar r_nightvisionambient( "r_nightvisionambient", "0.0", FCVAR_CHEAT );
static ConVar r_nightvisionshadowatten( "r_nightvisionshadowatten", "0.35", FCVAR_CHEAT );
static ConVar r_nightvisionladderdist( "r_nightvisionladderdist", "40.0", FCVAR_CHEAT );
static ConVar mat_slopescaledepthbias_shadowmap( "mat_slopescaledepthbias_shadowmap", "16", FCVAR_CHEAT );
static ConVar mat_depthbias_shadowmap(	"mat_depthbias_shadowmap", "0.0005", FCVAR_CHEAT  );


void r_newnightvisionCallback_f( IConVar *pConVar, const char *pOldString, float flOldValue )
{
	if( engine->GetDXSupportLevel() < 70 )
	{
		r_newnightvision.SetValue( 0 );
	}	
}

//-----------------------------------------------------------------------------
// Purpose: 
// Input  : nEntIndex - The m_nEntIndex of the client entity that is creating us.
//			vecPos - The position of the light emitter.
//			vecDir - The direction of the light emission.
//-----------------------------------------------------------------------------
CFlashlightEffect::CFlashlightEffect(int nEntIndex)
{
	m_FlashlightHandle = CLIENTSHADOW_INVALID_HANDLE;
	m_nEntIndex = nEntIndex;

	m_bIsOn = false;
	m_pPointLight = NULL;
	if( engine->GetDXSupportLevel() < 70 )
	{
		r_newnightvision.SetValue( 0 );
	}	

	if ( g_pMaterialSystemHardwareConfig->SupportsBorderColor() )
	{
		m_FlashlightTexture.Init( "effects/flashlight_border", TEXTURE_GROUP_OTHER, true );
	}
	else
	{
		m_FlashlightTexture.Init( "effects/flashlight001", TEXTURE_GROUP_OTHER, true );
	}
}


//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
CFlashlightEffect::~CFlashlightEffect()
{
	LightOff();
}


//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CFlashlightEffect::TurnOn()
{
	m_bIsOn = true;
	m_flDistMod = 1.0f;
	IMaterial *pMaterial = materials->FindMaterial("hudoverlays/NightVision", TEXTURE_GROUP_OTHER, true); //set pMaterial to our texture
	view->SetScreenOverlayMaterial(pMaterial); //and overlay it on the screen
	CLocalPlayerFilter filter;
}


//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CFlashlightEffect::TurnOff()
{
	if (m_bIsOn)
	{
		m_bIsOn = false;
		LightOff();
	}
}

// Custom trace filter that skips the player and the view model.
// If we don't do this, we'll end up having the light right in front of us all
// the time.
class CTraceFilterSkipPlayerAndViewModel : public CTraceFilter
{
public:
	virtual bool ShouldHitEntity( IHandleEntity *pServerEntity, int contentsMask )
	{
		// Test against the vehicle too?
		// FLASHLIGHTFIXME: how do you know that you are actually inside of the vehicle?
		C_BaseEntity *pEntity = EntityFromEntityHandle( pServerEntity );
		if ( !pEntity )
			return true;

		if ( ( dynamic_cast<C_BaseViewModel *>( pEntity ) != NULL ) ||
			 ( dynamic_cast<C_BasePlayer *>( pEntity ) != NULL ) ||
			 pEntity->GetCollisionGroup() == COLLISION_GROUP_DEBRIS ||
			 pEntity->GetCollisionGroup() == COLLISION_GROUP_INTERACTIVE_DEBRIS )
		{
			return false;
		}

		return true;
	}
};

//-----------------------------------------------------------------------------
// Purpose: Do the headlight
//-----------------------------------------------------------------------------
void CFlashlightEffect::UpdateLightNew(const Vector &vecPos, const Vector &vecForward, const Vector &vecRight, const Vector &vecUp )
{
	VPROF_BUDGET( "CFlashlightEffect::UpdateLightNew", VPROF_BUDGETGROUP_SHADOW_DEPTH_TEXTURING );

	FlashlightState_t state;

	// We will lock some of the nightvision params if player is on a ladder, to prevent oscillations due to the trace-rays
	bool bPlayerOnLadder = ( C_BasePlayer::GetLocalPlayer()->GetMoveType() == MOVETYPE_LADDER );

	const float flEpsilon = 0.1f;			// Offset nightvision position along vecUp
	const float flDistCutoff = 128.0f;
	const float flDistDrag = 0.2;

	CTraceFilterSkipPlayerAndViewModel traceFilter;
	float flOffsetY = r_nightvisionoffsety.GetFloat();

	if( r_swingnightvision.GetBool() )
	{
		// This projects the view direction backwards, attempting to raise the vertical
		// offset of the nightvision, but only when the player is looking down.
		Vector vecSwingLight = vecPos + vecForward * -12.0f;
		if( vecSwingLight.z > vecPos.z )
		{
			flOffsetY += (vecSwingLight.z - vecPos.z);
		}
	}

	Vector vOrigin = vecPos + flOffsetY * vecUp;

	// Not on ladder...trace a hull
	if ( !bPlayerOnLadder ) 
	{
		trace_t pmOriginTrace;
		UTIL_TraceHull( vecPos, vOrigin, Vector(-4, -4, -4), Vector(4, 4, 4), MASK_SOLID & ~(CONTENTS_HITBOX), &traceFilter, &pmOriginTrace );

		if ( pmOriginTrace.DidHit() )
		{
			vOrigin = vecPos;
		}
	}
	else // on ladder...skip the above hull trace
	{
		vOrigin = vecPos;
	}

	// Now do a trace along the nightvision direction to ensure there is nothing within range to pull back from
	int iMask = MASK_OPAQUE_AND_NPCS;
	iMask &= ~CONTENTS_HITBOX;
	iMask |= CONTENTS_WINDOW;

	Vector vTarget = vecPos + vecForward * 1024.0f;

	// Work with these local copies of the basis for the rest of the function
	Vector vDir   = vTarget - vOrigin;
	Vector vRight = vecRight;
	Vector vUp    = vecUp;
	VectorNormalize( vDir   );
	VectorNormalize( vRight );
	VectorNormalize( vUp    );

	// Orthonormalize the basis, since the nightvision texture projection will require this later...
	vUp -= DotProduct( vDir, vUp ) * vDir;
	VectorNormalize( vUp );
	vRight -= DotProduct( vDir, vRight ) * vDir;
	VectorNormalize( vRight );
	vRight -= DotProduct( vUp, vRight ) * vUp;
	VectorNormalize( vRight );

	AssertFloatEquals( DotProduct( vDir, vRight ), 0.0f, 1e-3 );
	AssertFloatEquals( DotProduct( vDir, vUp    ), 0.0f, 1e-3 );
	AssertFloatEquals( DotProduct( vRight, vUp  ), 0.0f, 1e-3 );

	trace_t pmDirectionTrace;
	UTIL_TraceHull( vOrigin, vTarget, Vector( -4, -4, -4 ), Vector( 4, 4, 4 ), iMask, &traceFilter, &pmDirectionTrace );

	if ( r_nightvisionvisualizetrace.GetBool() == true )
	{
		debugoverlay->AddBoxOverlay( pmDirectionTrace.endpos, Vector( -4, -4, -4 ), Vector( 4, 4, 4 ), QAngle( 0, 0, 0 ), 0, 0, 255, 16, 0 );
		debugoverlay->AddLineOverlay( vOrigin, pmDirectionTrace.endpos, 255, 0, 0, false, 0 );
	}

	float flDist = (pmDirectionTrace.endpos - vOrigin).Length();
	if ( flDist < flDistCutoff )
	{
		// We have an intersection with our cutoff range
		// Determine how far to pull back, then trace to see if we are clear
		float flPullBackDist = bPlayerOnLadder ? r_nightvisionladderdist.GetFloat() : flDistCutoff - flDist;	// Fixed pull-back distance if on ladder
		m_flDistMod = Lerp( flDistDrag, m_flDistMod, flPullBackDist );
		
		if ( !bPlayerOnLadder )
		{
			trace_t pmBackTrace;
			UTIL_TraceHull( vOrigin, vOrigin - vDir*(flPullBackDist-flEpsilon), Vector( -4, -4, -4 ), Vector( 4, 4, 4 ), iMask, &traceFilter, &pmBackTrace );
			if( pmBackTrace.DidHit() )
			{
				// We have an intersection behind us as well, so limit our m_flDistMod
				float flMaxDist = (pmBackTrace.endpos - vOrigin).Length() - flEpsilon;
				if( m_flDistMod > flMaxDist )
					m_flDistMod = flMaxDist;
			}
		}
	}
	else
	{
		m_flDistMod = Lerp( flDistDrag, m_flDistMod, 0.0f );
	}
	vOrigin = vOrigin - vDir * m_flDistMod;

	state.m_vecLightOrigin = vOrigin;

	BasisToQuaternion( vDir, vRight, vUp, state.m_quatOrientation );

	state.m_fQuadraticAtten = r_nightvisionquadratic.GetFloat();

	bool bFlicker = false;

	if ( bFlicker == false )
	{
		state.m_fLinearAtten = r_nightvisionlinear.GetFloat();
		state.m_fHorizontalFOVDegrees = 179.0f;
		state.m_fVerticalFOVDegrees = 179.0f;
	}

	state.m_fConstantAtten = r_nightvisionconstant.GetFloat();
	state.m_Color[0] = 0.0f;
	state.m_Color[1] = 1.0f;
	state.m_Color[2] = 0.0f;
	state.m_Color[3] = r_nightvisionambient.GetFloat();
	state.m_NearZ = r_nightvisionnear.GetFloat() + m_flDistMod;	// Push near plane out so that we don't clip the world when the nightvision pulls back 
	state.m_FarZ = 1024.f;
	state.m_bEnableShadows = false;
	state.m_flShadowMapResolution = 0;

	state.m_pSpotlightTexture = m_FlashlightTexture;
	state.m_nSpotlightTextureFrame = 0;

	state.m_flShadowAtten = r_nightvisionshadowatten.GetFloat();
	state.m_flShadowSlopeScaleDepthBias = mat_slopescaledepthbias_shadowmap.GetFloat();
	state.m_flShadowDepthBias = mat_depthbias_shadowmap.GetFloat();

	if( m_FlashlightHandle == CLIENTSHADOW_INVALID_HANDLE )
	{
		m_FlashlightHandle = g_pClientShadowMgr->CreateFlashlight( state );
	}
	else
	{
		if( !r_nightvisionlockposition.GetBool() )
		{
			g_pClientShadowMgr->UpdateFlashlightState( m_FlashlightHandle, state );
		}
	}
	
	g_pClientShadowMgr->UpdateProjectedTexture( m_FlashlightHandle, true );
	
	// Kill the old nightvision method if we have one.
	LightOffOld();

#ifndef NO_TOOLFRAMEWORK
	if ( clienttools->IsInRecordingMode() )
	{
		KeyValues *msg = new KeyValues( "FlashlightState" );
		msg->SetFloat( "time", gpGlobals->curtime );
		msg->SetInt( "entindex", m_nEntIndex );
		msg->SetInt( "flashlightHandle", m_FlashlightHandle );
		msg->SetPtr( "flashlightState", &state );
		ToolFramework_PostToolMessage( HTOOLHANDLE_INVALID, msg );
		msg->deleteThis();
	}
#endif
}

//-----------------------------------------------------------------------------
// Purpose: Do the headlight
//-----------------------------------------------------------------------------
void CFlashlightEffect::UpdateLightOld(const Vector &vecPos, const Vector &vecDir, int nDistance)
{
	if ( !m_pPointLight || ( m_pPointLight->key != m_nEntIndex ))
	{
		// Set up the environment light
		m_pPointLight = effects->CL_AllocDlight(m_nEntIndex);
		m_pPointLight->flags = 0.0f;
		m_pPointLight->radius = 80;
	}
	
	// For bumped lighting
	VectorCopy(vecDir, m_pPointLight->m_Direction);
	
	Vector end;
	end = vecPos + nDistance * vecDir;
	
	// Trace a line outward, skipping the player model and the view model.
	trace_t pm;
	CTraceFilterSkipPlayerAndViewModel traceFilter;
	UTIL_TraceLine( vecPos, end, MASK_ALL, &traceFilter, &pm );
	VectorCopy( pm.endpos, m_pPointLight->origin );
	
	float falloff = pm.fraction * nDistance;
	
	if ( falloff < 500 )
		falloff = 1.0;
	else
		falloff = 500.0 / falloff;
	
	falloff *= falloff;
	
	m_pPointLight->radius = 80;
	m_pPointLight->color.r = m_pPointLight->color.g = m_pPointLight->color.b = 255 * falloff;
	m_pPointLight->color.exponent = 0;
	
	// Make it live for a bit
	m_pPointLight->die = gpGlobals->curtime + 0.2f;
	
	// Update list of surfaces we influence
	render->TouchLight( m_pPointLight );
	
	// kill the new nightvision if we have one
	LightOffNew();
}

//-----------------------------------------------------------------------------
// Purpose: Do the headlight
//-----------------------------------------------------------------------------
void CFlashlightEffect::UpdateLight(const Vector &vecPos, const Vector &vecDir, const Vector &vecRight, const Vector &vecUp, int nDistance)
{
	if ( !m_bIsOn )
	{
		return;
	}
	if( r_newnightvision.GetBool() )
	{
		UpdateLightNew( vecPos, vecDir, vecRight, vecUp );
	}
	else
	{
		UpdateLightOld( vecPos, vecDir, nDistance );
	}
}


//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CFlashlightEffect::LightOffNew()
{
#ifndef NO_TOOLFRAMEWORK
	if ( clienttools->IsInRecordingMode() )
	{
		KeyValues *msg = new KeyValues( "FlashlightState" );
		msg->SetFloat( "time", gpGlobals->curtime );
		msg->SetInt( "entindex", m_nEntIndex );
		msg->SetInt( "flashlightHandle", m_FlashlightHandle );
		msg->SetPtr( "flashlightState", NULL );
		ToolFramework_PostToolMessage( HTOOLHANDLE_INVALID, msg );
		msg->deleteThis();
	}
#endif

	// Clear out the light
	if( m_FlashlightHandle != CLIENTSHADOW_INVALID_HANDLE )
	{
		g_pClientShadowMgr->DestroyFlashlight( m_FlashlightHandle );
		m_FlashlightHandle = CLIENTSHADOW_INVALID_HANDLE;
	}

	view->SetScreenOverlayMaterial(null); //set screenoverlay to nothing
	CLocalPlayerFilter filter;
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CFlashlightEffect::LightOffOld()
{	
	if ( m_pPointLight && ( m_pPointLight->key == m_nEntIndex ) )
	{
		m_pPointLight->die = gpGlobals->curtime;
		m_pPointLight = NULL;
	}
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CFlashlightEffect::LightOff()
{	
	LightOffOld();
	LightOffNew();
}

CHeadlightEffect::CHeadlightEffect() 
{

}

CHeadlightEffect::~CHeadlightEffect()
{
	
}

void CHeadlightEffect::UpdateLight( const Vector &vecPos, const Vector &vecDir, const Vector &vecRight, const Vector &vecUp, int nDistance )
{
	if ( IsOn() == false )
		 return;

	FlashlightState_t state;
	Vector basisX, basisY, basisZ;
	basisX = vecDir;
	basisY = vecRight;
	basisZ = vecUp;
	VectorNormalize(basisX);
	VectorNormalize(basisY);
	VectorNormalize(basisZ);

	BasisToQuaternion( basisX, basisY, basisZ, state.m_quatOrientation );
		
	state.m_vecLightOrigin = vecPos;

	state.m_fHorizontalFOVDegrees = 45.0f;
	state.m_fVerticalFOVDegrees = 30.0f;
	state.m_fQuadraticAtten = r_nightvisionquadratic.GetFloat();
	state.m_fLinearAtten = r_nightvisionlinear.GetFloat();
	state.m_fConstantAtten = r_nightvisionconstant.GetFloat();
	state.m_Color[0] = 1.0f;
	state.m_Color[1] = 1.0f;
	state.m_Color[2] = 1.0f;
	state.m_Color[3] = r_nightvisionambient.GetFloat();
	state.m_NearZ = r_nightvisionnear.GetFloat();
	state.m_FarZ = r_nightvisionfar.GetFloat();
	state.m_bEnableShadows = true;
	state.m_pSpotlightTexture = m_FlashlightTexture;
	state.m_nSpotlightTextureFrame = 0;
	
	if( GetFlashlightHandle() == CLIENTSHADOW_INVALID_HANDLE )
	{
		SetFlashlightHandle( g_pClientShadowMgr->CreateFlashlight( state ) );
	}
	else
	{
		g_pClientShadowMgr->UpdateFlashlightState( GetFlashlightHandle(), state );
	}
	
	g_pClientShadowMgr->UpdateProjectedTexture( GetFlashlightHandle(), true );
}

