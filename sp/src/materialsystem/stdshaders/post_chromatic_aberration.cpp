#include "BaseVSShader.h"

#include "PassThrough_vs30.inc"
#include "post_chromatic_aberration_ps30.inc"

BEGIN_VS_SHADER( Post_Chromatic_Aberration, "Help for Post_Chromatic_Aberration" )

	BEGIN_SHADER_PARAMS
	SHADER_PARAM(DISTANCE, SHADER_PARAM_TYPE_FLOAT, "1.0", "")
	SHADER_PARAM( FBTEXTURE, SHADER_PARAM_TYPE_TEXTURE, "_rt_FullFrameFB", "" )
	END_SHADER_PARAMS

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
			pShaderShadow->SetPixelShader( "post_chromatic_aberration_ps30" );
				
			DefaultFog();
		}

		DYNAMIC_STATE
		{
			float Scale = params[DISTANCE]->GetFloatValue();
			float vScale[4] = {Scale, Scale, Scale, 1};
			pShaderAPI->SetPixelShaderConstant(0, vScale);
			BindTexture( SHADER_SAMPLER0, FBTEXTURE, -1 );
		}
		Draw();
	}
END_SHADER