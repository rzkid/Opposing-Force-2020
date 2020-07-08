// ALL SKIP STATEMENTS THAT AFFECT THIS SHADER!!!
// defined $LIGHTING_PREVIEW && defined $FASTPATH && $LIGHTING_PREVIEW && $FASTPATH

#ifndef PASSTHROUGH_VS30_H
#define PASSTHROUGH_VS30_H

#include "shaderapi/ishaderapi.h"
#include "shaderapi/ishadershadow.h"
#include "materialsystem/imaterialvar.h"

class PassThrough_vs30_Static_Index
{
public:
	PassThrough_vs30_Static_Index( IShaderShadow* pShaderShadow, IMaterialVar** params )
	{
	}

	int GetIndex() const
	{
		return 0;
	}
};

#define shaderStaticTest_PassThrough_vs30 1


class PassThrough_vs30_Dynamic_Index
{
public:
	PassThrough_vs30_Dynamic_Index( IShaderDynamicAPI* pShaderAPI )
	{
	}

	int GetIndex() const
	{
		return 0;
	}
};

#define shaderDynamicTest_PassThrough_vs30 1


#endif	// PASSTHROUGH_VS30_H