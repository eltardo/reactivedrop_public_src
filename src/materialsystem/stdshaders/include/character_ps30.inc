// ALL SKIP STATEMENTS THAT AFFECT THIS SHADER!!!
// ( $DETAILTEXTURE == 0 ) && ( $DETAIL_BLEND_MODE != 0 )
// ( $FLASHLIGHT == 0 ) && ( $FLASHLIGHTSHADOWS == 1 )
// ( $FLASHLIGHT == 0 ) && ( $UBERLIGHT == 1 )
// ( $SELFILLUM == 0 ) && ( $SELFILLUMFRESNEL == 1 )
// ( $FLASHLIGHT == 1 ) && ( $SELFILLUMFRESNEL == 1 )
// ( $FLASHLIGHT == 1 ) && ( $SELFILLUM == 1 )
// ( $WORLD_NORMAL == 1 ) && ( $FLASHLIGHTSHADOWS == 1 ) && ( $NUM_LIGHTS != 0 ) && ( $WRITEWATERFOGTODESTALPHA == 1 )
// ( $WRINKLEMAP == 1 ) && ( $STATUSEFFECT != 0 )
// defined $PIXELFOGTYPE && defined $WRITEWATERFOGTODESTALPHA && ( $PIXELFOGTYPE != 1 ) && $WRITEWATERFOGTODESTALPHA
// defined $LIGHTING_PREVIEW && defined $FASTPATHENVMAPTINT && $LIGHTING_PREVIEW && $FASTPATHENVMAPTINT
// defined $LIGHTING_PREVIEW && defined $FASTPATHENVMAPCONTRAST && $LIGHTING_PREVIEW && $FASTPATHENVMAPCONTRAST
// defined $LIGHTING_PREVIEW && defined $FASTPATH && $LIGHTING_PREVIEW && $FASTPATH
// ($FLASHLIGHT || $FLASHLIGHTSHADOWS) && $LIGHTING_PREVIEW
// defined $PIXELFOGTYPE && defined $WRITEWATERFOGTODESTALPHA && ( $PIXELFOGTYPE != 1 ) && $WRITEWATERFOGTODESTALPHA
// defined $LIGHTING_PREVIEW && defined $FASTPATHENVMAPTINT && $LIGHTING_PREVIEW && $FASTPATHENVMAPTINT
// defined $LIGHTING_PREVIEW && defined $FASTPATHENVMAPCONTRAST && $LIGHTING_PREVIEW && $FASTPATHENVMAPCONTRAST
// defined $LIGHTING_PREVIEW && defined $FASTPATH && $LIGHTING_PREVIEW && $FASTPATH
// ($FLASHLIGHT || $FLASHLIGHTSHADOWS) && $LIGHTING_PREVIEW

#pragma once
#include "shaderlib/cshader.h"
class character_ps30_Static_Index
{
	unsigned int m_nCUBEMAP : 2;
	unsigned int m_nSELFILLUM : 2;
	unsigned int m_nSELFILLUMFRESNEL : 2;
	unsigned int m_nFLASHLIGHT : 2;
	unsigned int m_nLIGHTWARPTEXTURE : 2;
	unsigned int m_nPHONGWARPTEXTURE : 2;
	unsigned int m_nDETAIL_BLEND_MODE : 3;
	unsigned int m_nDETAILTEXTURE : 2;
	unsigned int m_nRIMLIGHT : 2;
	unsigned int m_nFLASHLIGHTDEPTHFILTERMODE : 2;
	unsigned int m_nWORLD_NORMAL : 2;
	unsigned int m_nPHONG_HALFLAMBERT : 2;
#ifdef _DEBUG
	bool m_bCUBEMAP : 1;
	bool m_bSELFILLUM : 1;
	bool m_bSELFILLUMFRESNEL : 1;
	bool m_bFLASHLIGHT : 1;
	bool m_bLIGHTWARPTEXTURE : 1;
	bool m_bPHONGWARPTEXTURE : 1;
	bool m_bDETAIL_BLEND_MODE : 1;
	bool m_bDETAILTEXTURE : 1;
	bool m_bRIMLIGHT : 1;
	bool m_bFLASHLIGHTDEPTHFILTERMODE : 1;
	bool m_bWORLD_NORMAL : 1;
	bool m_bPHONG_HALFLAMBERT : 1;
#endif	// _DEBUG
public:
	void SetCUBEMAP( int i )
	{
		Assert( i >= 0 && i <= 1 );
		m_nCUBEMAP = i;
#ifdef _DEBUG
		m_bCUBEMAP = true;
#endif	// _DEBUG
	}

	void SetSELFILLUM( int i )
	{
		Assert( i >= 0 && i <= 1 );
		m_nSELFILLUM = i;
#ifdef _DEBUG
		m_bSELFILLUM = true;
#endif	// _DEBUG
	}

	void SetSELFILLUMFRESNEL( int i )
	{
		Assert( i >= 0 && i <= 1 );
		m_nSELFILLUMFRESNEL = i;
#ifdef _DEBUG
		m_bSELFILLUMFRESNEL = true;
#endif	// _DEBUG
	}

	void SetFLASHLIGHT( int i )
	{
		Assert( i >= 0 && i <= 1 );
		m_nFLASHLIGHT = i;
#ifdef _DEBUG
		m_bFLASHLIGHT = true;
#endif	// _DEBUG
	}

	void SetLIGHTWARPTEXTURE( int i )
	{
		Assert( i >= 0 && i <= 1 );
		m_nLIGHTWARPTEXTURE = i;
#ifdef _DEBUG
		m_bLIGHTWARPTEXTURE = true;
#endif	// _DEBUG
	}

	void SetPHONGWARPTEXTURE( int i )
	{
		Assert( i >= 0 && i <= 1 );
		m_nPHONGWARPTEXTURE = i;
#ifdef _DEBUG
		m_bPHONGWARPTEXTURE = true;
#endif	// _DEBUG
	}

	void SetDETAIL_BLEND_MODE( int i )
	{
		Assert( i >= 0 && i <= 6 );
		m_nDETAIL_BLEND_MODE = i;
#ifdef _DEBUG
		m_bDETAIL_BLEND_MODE = true;
#endif	// _DEBUG
	}

	void SetDETAILTEXTURE( int i )
	{
		Assert( i >= 0 && i <= 1 );
		m_nDETAILTEXTURE = i;
#ifdef _DEBUG
		m_bDETAILTEXTURE = true;
#endif	// _DEBUG
	}

	void SetRIMLIGHT( int i )
	{
		Assert( i >= 0 && i <= 1 );
		m_nRIMLIGHT = i;
#ifdef _DEBUG
		m_bRIMLIGHT = true;
#endif	// _DEBUG
	}

	void SetFLASHLIGHTDEPTHFILTERMODE( int i )
	{
		Assert( i >= 0 && i <= 2 );
		m_nFLASHLIGHTDEPTHFILTERMODE = i;
#ifdef _DEBUG
		m_bFLASHLIGHTDEPTHFILTERMODE = true;
#endif	// _DEBUG
	}

	void SetWORLD_NORMAL( int i )
	{
		Assert( i >= 0 && i <= 1 );
		m_nWORLD_NORMAL = i;
#ifdef _DEBUG
		m_bWORLD_NORMAL = true;
#endif	// _DEBUG
	}

	void SetPHONG_HALFLAMBERT( int i )
	{
		Assert( i >= 0 && i <= 1 );
		m_nPHONG_HALFLAMBERT = i;
#ifdef _DEBUG
		m_bPHONG_HALFLAMBERT = true;
#endif	// _DEBUG
	}

	character_ps30_Static_Index( IShaderShadow* pShaderShadow, IMaterialVar** params )
	{
		m_nCUBEMAP = 0;
		m_nSELFILLUM = 0;
		m_nSELFILLUMFRESNEL = 0;
		m_nFLASHLIGHT = 0;
		m_nLIGHTWARPTEXTURE = 0;
		m_nPHONGWARPTEXTURE = 0;
		m_nDETAIL_BLEND_MODE = 0;
		m_nDETAILTEXTURE = 0;
		m_nRIMLIGHT = 0;
		m_nFLASHLIGHTDEPTHFILTERMODE = 0;
		m_nWORLD_NORMAL = 0;
		m_nPHONG_HALFLAMBERT = 0;
#ifdef _DEBUG
		m_bCUBEMAP = false;
		m_bSELFILLUM = false;
		m_bSELFILLUMFRESNEL = false;
		m_bFLASHLIGHT = false;
		m_bLIGHTWARPTEXTURE = false;
		m_bPHONGWARPTEXTURE = false;
		m_bDETAIL_BLEND_MODE = false;
		m_bDETAILTEXTURE = false;
		m_bRIMLIGHT = false;
		m_bFLASHLIGHTDEPTHFILTERMODE = false;
		m_bWORLD_NORMAL = false;
		m_bPHONG_HALFLAMBERT = false;
#endif	// _DEBUG
	}

	int GetIndex() const
	{
		Assert( m_bCUBEMAP && m_bSELFILLUM && m_bSELFILLUMFRESNEL && m_bFLASHLIGHT && m_bLIGHTWARPTEXTURE && m_bPHONGWARPTEXTURE && m_bDETAIL_BLEND_MODE && m_bDETAILTEXTURE && m_bRIMLIGHT && m_bFLASHLIGHTDEPTHFILTERMODE && m_bWORLD_NORMAL && m_bPHONG_HALFLAMBERT );
		AssertMsg( !( ( m_nDETAILTEXTURE == 0 ) && ( m_nDETAIL_BLEND_MODE != 0 ) ), "Invalid combo combination ( ( DETAILTEXTURE == 0 ) && ( DETAIL_BLEND_MODE != 0 ) )" );
		AssertMsg( !( ( m_nSELFILLUM == 0 ) && ( m_nSELFILLUMFRESNEL == 1 ) ), "Invalid combo combination ( ( SELFILLUM == 0 ) && ( SELFILLUMFRESNEL == 1 ) )" );
		AssertMsg( !( ( m_nFLASHLIGHT == 1 ) && ( m_nSELFILLUMFRESNEL == 1 ) ), "Invalid combo combination ( ( FLASHLIGHT == 1 ) && ( SELFILLUMFRESNEL == 1 ) )" );
		AssertMsg( !( ( m_nFLASHLIGHT == 1 ) && ( m_nSELFILLUM == 1 ) ), "Invalid combo combination ( ( FLASHLIGHT == 1 ) && ( SELFILLUM == 1 ) )" );
		return ( 1280 * m_nCUBEMAP ) + ( 2560 * m_nSELFILLUM ) + ( 5120 * m_nSELFILLUMFRESNEL ) + ( 10240 * m_nFLASHLIGHT ) + ( 20480 * m_nLIGHTWARPTEXTURE ) + ( 40960 * m_nPHONGWARPTEXTURE ) + ( 81920 * m_nDETAIL_BLEND_MODE ) + ( 573440 * m_nDETAILTEXTURE ) + ( 1146880 * m_nRIMLIGHT ) + ( 2293760 * m_nFLASHLIGHTDEPTHFILTERMODE ) + ( 6881280 * m_nWORLD_NORMAL ) + ( 13762560 * m_nPHONG_HALFLAMBERT ) + 0;
	}
};

#define shaderStaticTest_character_ps30 psh_forgot_to_set_static_CUBEMAP + psh_forgot_to_set_static_SELFILLUM + psh_forgot_to_set_static_SELFILLUMFRESNEL + psh_forgot_to_set_static_FLASHLIGHT + psh_forgot_to_set_static_LIGHTWARPTEXTURE + psh_forgot_to_set_static_PHONGWARPTEXTURE + psh_forgot_to_set_static_DETAIL_BLEND_MODE + psh_forgot_to_set_static_DETAILTEXTURE + psh_forgot_to_set_static_RIMLIGHT + psh_forgot_to_set_static_FLASHLIGHTDEPTHFILTERMODE + psh_forgot_to_set_static_WORLD_NORMAL + psh_forgot_to_set_static_PHONG_HALFLAMBERT


class character_ps30_Dynamic_Index
{
	unsigned int m_nWRITEWATERFOGTODESTALPHA : 2;
	unsigned int m_nPIXELFOGTYPE : 2;
	unsigned int m_nNUM_LIGHTS : 3;
	unsigned int m_nWRITE_DEPTH_TO_DESTALPHA : 2;
	unsigned int m_nFLASHLIGHTSHADOWS : 2;
	unsigned int m_nUBERLIGHT : 2;
	unsigned int m_nWRINKLEMAP : 2;
	unsigned int m_nSTATUSEFFECT : 2;
	unsigned int m_nTEAMGLOW : 2;
#ifdef _DEBUG
	bool m_bWRITEWATERFOGTODESTALPHA : 1;
	bool m_bNUM_LIGHTS : 1;
	bool m_bWRITE_DEPTH_TO_DESTALPHA : 1;
	bool m_bFLASHLIGHTSHADOWS : 1;
	bool m_bUBERLIGHT : 1;
	bool m_bWRINKLEMAP : 1;
	bool m_bSTATUSEFFECT : 1;
	bool m_bTEAMGLOW : 1;
#endif	// _DEBUG
public:
	void SetWRITEWATERFOGTODESTALPHA( int i )
	{
		Assert( i >= 0 && i <= 1 );
		m_nWRITEWATERFOGTODESTALPHA = i;
#ifdef _DEBUG
		m_bWRITEWATERFOGTODESTALPHA = true;
#endif	// _DEBUG
	}

	void SetPIXELFOGTYPE( int i )
	{
		Assert( i >= 0 && i <= 1 );
		m_nPIXELFOGTYPE = i;
	}

	void SetNUM_LIGHTS( int i )
	{
		Assert( i >= 0 && i <= 4 );
		m_nNUM_LIGHTS = i;
#ifdef _DEBUG
		m_bNUM_LIGHTS = true;
#endif	// _DEBUG
	}

	void SetWRITE_DEPTH_TO_DESTALPHA( int i )
	{
		Assert( i >= 0 && i <= 1 );
		m_nWRITE_DEPTH_TO_DESTALPHA = i;
#ifdef _DEBUG
		m_bWRITE_DEPTH_TO_DESTALPHA = true;
#endif	// _DEBUG
	}

	void SetFLASHLIGHTSHADOWS( int i )
	{
		Assert( i >= 0 && i <= 1 );
		m_nFLASHLIGHTSHADOWS = i;
#ifdef _DEBUG
		m_bFLASHLIGHTSHADOWS = true;
#endif	// _DEBUG
	}

	void SetUBERLIGHT( int i )
	{
		Assert( i >= 0 && i <= 1 );
		m_nUBERLIGHT = i;
#ifdef _DEBUG
		m_bUBERLIGHT = true;
#endif	// _DEBUG
	}

	void SetWRINKLEMAP( int i )
	{
		Assert( i >= 0 && i <= 1 );
		m_nWRINKLEMAP = i;
#ifdef _DEBUG
		m_bWRINKLEMAP = true;
#endif	// _DEBUG
	}

	void SetSTATUSEFFECT( int i )
	{
		Assert( i >= 0 && i <= 1 );
		m_nSTATUSEFFECT = i;
#ifdef _DEBUG
		m_bSTATUSEFFECT = true;
#endif	// _DEBUG
	}

	void SetTEAMGLOW( int i )
	{
		Assert( i >= 0 && i <= 1 );
		m_nTEAMGLOW = i;
#ifdef _DEBUG
		m_bTEAMGLOW = true;
#endif	// _DEBUG
	}

	character_ps30_Dynamic_Index( IShaderDynamicAPI* pShaderAPI )
	{
		m_nWRITEWATERFOGTODESTALPHA = 0;
		m_nPIXELFOGTYPE = ( pShaderAPI->GetSceneFogMode() == MATERIAL_FOG_LINEAR_BELOW_FOG_Z ) ;
		m_nNUM_LIGHTS = 0;
		m_nWRITE_DEPTH_TO_DESTALPHA = 0;
		m_nFLASHLIGHTSHADOWS = 0;
		m_nUBERLIGHT = 0;
		m_nWRINKLEMAP = 0;
		m_nSTATUSEFFECT = 0;
		m_nTEAMGLOW = 0;
#ifdef _DEBUG
		m_bWRITEWATERFOGTODESTALPHA = false;
		m_bNUM_LIGHTS = false;
		m_bWRITE_DEPTH_TO_DESTALPHA = false;
		m_bFLASHLIGHTSHADOWS = false;
		m_bUBERLIGHT = false;
		m_bWRINKLEMAP = false;
		m_bSTATUSEFFECT = false;
		m_bTEAMGLOW = false;
#endif	// _DEBUG
	}

	int GetIndex() const
	{
		Assert( m_bWRITEWATERFOGTODESTALPHA && m_bNUM_LIGHTS && m_bWRITE_DEPTH_TO_DESTALPHA && m_bFLASHLIGHTSHADOWS && m_bUBERLIGHT && m_bWRINKLEMAP && m_bSTATUSEFFECT && m_bTEAMGLOW );
		AssertMsg( !( ( m_nWRINKLEMAP == 1 ) && ( m_nSTATUSEFFECT != 0 ) ), "Invalid combo combination ( ( WRINKLEMAP == 1 ) && ( STATUSEFFECT != 0 ) )" );
		AssertMsg( !( 1 && ( 1 && ( ( m_nPIXELFOGTYPE != 1 ) && m_nWRITEWATERFOGTODESTALPHA ) ) ), "Invalid combo combination ( 1 && ( 1 && ( ( PIXELFOGTYPE != 1 ) && WRITEWATERFOGTODESTALPHA ) ) )" );
		AssertMsg( !( 1 && ( 1 && ( ( m_nPIXELFOGTYPE != 1 ) && m_nWRITEWATERFOGTODESTALPHA ) ) ), "Invalid combo combination ( 1 && ( 1 && ( ( PIXELFOGTYPE != 1 ) && WRITEWATERFOGTODESTALPHA ) ) )" );
		return ( 1 * m_nWRITEWATERFOGTODESTALPHA ) + ( 2 * m_nPIXELFOGTYPE ) + ( 4 * m_nNUM_LIGHTS ) + ( 20 * m_nWRITE_DEPTH_TO_DESTALPHA ) + ( 40 * m_nFLASHLIGHTSHADOWS ) + ( 80 * m_nUBERLIGHT ) + ( 160 * m_nWRINKLEMAP ) + ( 320 * m_nSTATUSEFFECT ) + ( 640 * m_nTEAMGLOW ) + 0;
	}
};

#define shaderDynamicTest_character_ps30 psh_forgot_to_set_dynamic_WRITEWATERFOGTODESTALPHA + psh_forgot_to_set_dynamic_NUM_LIGHTS + psh_forgot_to_set_dynamic_WRITE_DEPTH_TO_DESTALPHA + psh_forgot_to_set_dynamic_FLASHLIGHTSHADOWS + psh_forgot_to_set_dynamic_UBERLIGHT + psh_forgot_to_set_dynamic_WRINKLEMAP + psh_forgot_to_set_dynamic_STATUSEFFECT + psh_forgot_to_set_dynamic_TEAMGLOW
