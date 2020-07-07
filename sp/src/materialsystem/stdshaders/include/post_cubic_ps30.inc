#ifndef POST_CUBIC_PS20B_H
#define POST_CUBIC_PS20B_H

#include "shaderapi/ishaderapi.h"
#include "shaderapi/ishadershadow.h"
#include "materialsystem/imaterialvar.h"

class post_cubic_ps20b_Static_Index
{
public:
	post_cubic_ps20b_Static_Index( IShaderShadow* pShaderShadow, IMaterialVar** params )
	{
	}

	int GetIndex() const
	{
		return 0;
	}
};

#define shaderStaticTest_post_cubic_ps20b 1


class post_cubic_ps20b_Dynamic_Index
{
public:
	post_cubic_ps20b_Dynamic_Index( IShaderDynamicAPI* pShaderAPI )
	{
	}

	int GetIndex() const
	{
		return 0;
	}
};

#define shaderDynamicTest_post_cubic_ps20b 1


#endif	// POST_CUBIC_PS20B_H