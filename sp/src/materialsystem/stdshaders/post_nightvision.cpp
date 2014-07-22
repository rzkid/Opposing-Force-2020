#include "BaseVSShader.h"

#include "PassThrough_vs30.inc"
#include "post_nightvision_ps30.inc"

ConVar mat_grain_scale_override( "mat_grain_scale_override", "2.0" );//was 4.2

BEGIN_VS_SHADER( Post_NightVision, "Help for Post_NightVision" )

	BEGIN_SHADER_PARAMS
	SHADER_PARAM( NVLEVEL,			SHADER_PARAM_TYPE_FLOAT,	"1.0",				"")
	SHADER_PARAM(INNER_RADIUS,		SHADER_PARAM_TYPE_FLOAT, ".38", "")
	SHADER_PARAM(OUTER_RADIUS,		SHADER_PARAM_TYPE_FLOAT, "45", "")
	SHADER_PARAM( FBTEXTURE,		SHADER_PARAM_TYPE_TEXTURE,	"_rt_FullFrameFB",	"" )
	SHADER_PARAM( NOISETEXTURE,		SHADER_PARAM_TYPE_TEXTURE,	"HUDoverlays/test_noise",					"Noise texture" )
	SHADER_PARAM( NOISESCALE,		SHADER_PARAM_TYPE_FLOAT,	"0",				"Noise scale" )
	END_SHADER_PARAMS

	SHADER_INIT_PARAMS()
	{
		if ( !params[ NOISESCALE ]->IsDefined() )
		{
			params[ NOISESCALE ]->SetFloatValue( 1.0f );
		}

		if ( !params[ NVLEVEL ]->IsDefined() )
		{
			params[ NVLEVEL]->SetFloatValue( 1.0f );
		}

		if ( !params[ FBTEXTURE ]->IsDefined() )
		{
			params[ FBTEXTURE ]->SetStringValue( "_rt_FullFrameFB" );
		}

		if ( !params[ NOISETEXTURE ]->IsDefined() )
		{
			params[ NOISETEXTURE ]->SetStringValue( "HUDoverlays/test_noise" );
		}

		if ( !params[ INNER_RADIUS ]->IsDefined() )
		{
			params[ INNER_RADIUS ]->SetFloatValue( 0.38f );
		}

		if ( !params[ OUTER_RADIUS ]->IsDefined() )
		{
			params[ OUTER_RADIUS ]->SetFloatValue( 0.45f );
		}
	}

	SHADER_FALLBACK
	{
		return 0;
	}

	SHADER_INIT
	{
		if( params[FBTEXTURE]->IsDefined() )
		{
			LoadTexture( FBTEXTURE );
		}
		if ( params[NOISETEXTURE]->IsDefined() )
		{
			LoadTexture( NOISETEXTURE );
		}
	}

	SHADER_DRAW
	{
		SHADOW_STATE
		{
			pShaderShadow->EnableDepthWrites( false );
			pShaderShadow->EnableTexture( SHADER_SAMPLER0, true );
			//pShaderShadow->EnableSRGBRead( SHADER_SAMPLER0, false );
			pShaderShadow->EnableTexture( SHADER_SAMPLER1, true );
			//pShaderShadow->EnableSRGBRead( SHADER_SAMPLER1, false );
 
			int fmt = VERTEX_POSITION;
			
			pShaderShadow->VertexShaderVertexFormat( fmt, 1, 0, 0 );
			pShaderShadow->SetVertexShader( "PassThrough_vs30", 0 );
			pShaderShadow->SetPixelShader( "post_nightvision_ps30" );
							
			DefaultFog();
		}

		DYNAMIC_STATE
		{
			BindTexture( SHADER_SAMPLER0, FBTEXTURE, -1 );
			BindTexture( SHADER_SAMPLER1, NOISETEXTURE );
			
			//========================================
			// night vision brightness level
			//========================================
			float Scale = params[NVLEVEL]->GetFloatValue();
			pShaderAPI->SetPixelShaderConstant(0, &Scale);
				
			//========================================
			// Noise scale
			//========================================
			float flNoiseScale = params[ NOISESCALE ]->GetFloatValue();
			if ( mat_grain_scale_override.GetFloat() != -1.0f ) 
				flNoiseScale = mat_grain_scale_override.GetFloat();
			if ( flNoiseScale < 0.0f ) 
				flNoiseScale = 0.0f;
			pShaderAPI->SetPixelShaderConstant( 1, &flNoiseScale );
			
			//========================================
			//	Time % 1000 for scrolling
			//========================================
			float flCurTime = 0.0f;
			float flTime = pShaderAPI->CurrentTime();
			flCurTime = flTime;
			flCurTime -= (float)( (int)( flCurTime / 1000.0f ) ) * 1000.0f;
			pShaderAPI->SetPixelShaderConstant(2, &flCurTime);
			
			//========================================
			// Outer vignette radius
			//========================================
			float outerRadius = params[OUTER_RADIUS]->GetFloatValue();
			pShaderAPI->SetPixelShaderConstant(3, &outerRadius);

			//========================================
			// Inner vignette radius
			//========================================
			float innerRadius = params[INNER_RADIUS]->GetFloatValue();
			pShaderAPI->SetPixelShaderConstant(4, &innerRadius);
		}
		Draw();
	}
END_SHADER