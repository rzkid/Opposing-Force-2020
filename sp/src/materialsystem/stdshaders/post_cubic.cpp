#include "BaseVSShader.h"

#include "PassThrough_vs30.inc"
#include "post_cubic_ps30.inc"

ConVar mat_cubic_k("mat_cubic_k","-1.0");
ConVar mat_cubic_kcube("mat_cubic_kcube","0.5");

BEGIN_VS_SHADER( Post_Cubic, "Help for Post_Cubic" )

	BEGIN_SHADER_PARAMS
	SHADER_PARAM( FBTEXTURE, SHADER_PARAM_TYPE_TEXTURE, "_rt_FullFrameFB", "" )
	END_SHADER_PARAMS

	SHADER_FALLBACK
	{
		return 0;
	}

	SHADER_INIT_PARAMS()
	{
		if ( !params[ FBTEXTURE ]->IsDefined() )
		{
			params[ FBTEXTURE ]->SetStringValue("_rt_FullFrameFB");
		}
	}

	SHADER_INIT
	{
		if( params[FBTEXTURE]->IsDefined() )
		{
			LoadTexture( FBTEXTURE );
		}
	}

	SHADER_DRAW
	{
		SHADOW_STATE
		{
			pShaderShadow->EnableDepthWrites( false );
			pShaderShadow->EnableTexture( SHADER_SAMPLER0, true ); 
 
			int fmt = VERTEX_POSITION;
			
			pShaderShadow->VertexShaderVertexFormat( fmt, 1, 0, 0 ); //sets the vertex format for the .fxc
			pShaderShadow->SetVertexShader( "PassThrough_vs30", 0 );
			pShaderShadow->SetPixelShader( "post_cubic_ps30" );
				
			DefaultFog();
		}

		DYNAMIC_STATE
		{
			BindTexture( SHADER_SAMPLER0, FBTEXTURE, -1 );
			float k = mat_cubic_k.GetFloat();
			pShaderAPI->SetPixelShaderConstant(0, &k);
			float kcube = mat_cubic_kcube.GetFloat();
			pShaderAPI->SetPixelShaderConstant(1, &kcube);
		}
		Draw();
	}
END_SHADER