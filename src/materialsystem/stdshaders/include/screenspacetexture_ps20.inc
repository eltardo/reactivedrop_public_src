// ALL SKIP STATEMENTS THAT AFFECT THIS SHADER!!!
// defined $PIXELFOGTYPE && defined $WRITEWATERFOGTODESTALPHA && ( $PIXELFOGTYPE != 1 ) && $WRITEWATERFOGTODESTALPHA
// defined $LIGHTING_PREVIEW && defined $FASTPATHENVMAPTINT && $LIGHTING_PREVIEW && $FASTPATHENVMAPTINT
// defined $LIGHTING_PREVIEW && defined $FASTPATHENVMAPCONTRAST && $LIGHTING_PREVIEW && $FASTPATHENVMAPCONTRAST
// defined $LIGHTING_PREVIEW && defined $FASTPATH && $LIGHTING_PREVIEW && $FASTPATH
// ($FLASHLIGHT || $FLASHLIGHTSHADOWS) && $LIGHTING_PREVIEW

#pragma once
#include "shaderlib/cshader.h"
class screenspacetexture_ps20_Static_Index
{
public:
	screenspacetexture_ps20_Static_Index( IShaderShadow *pShaderShadow, IMaterialVar **params )
	{
	}

	int GetIndex() const
	{
		return 0;
	}
};

#define shaderStaticTest_screenspacetexture_ps20 1


class screenspacetexture_ps20_Dynamic_Index
{
public:
	screenspacetexture_ps20_Dynamic_Index( IShaderDynamicAPI *pShaderAPI )
	{
	}

	int GetIndex() const
	{
		return 0;
	}
};

#define shaderDynamicTest_screenspacetexture_ps20 1