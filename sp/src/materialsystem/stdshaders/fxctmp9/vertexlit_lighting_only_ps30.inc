#include "shaderlib/cshader.h"
class vertexlit_lighting_only_ps30_Static_Index
{
private:
	int m_nDIFFUSELIGHTING;
#ifdef _DEBUG
	bool m_bDIFFUSELIGHTING;
#endif
public:
	void SetDIFFUSELIGHTING( int i )
	{
		Assert( i >= 0 && i <= 1 );
		m_nDIFFUSELIGHTING = i;
#ifdef _DEBUG
		m_bDIFFUSELIGHTING = true;
#endif
	}
	void SetDIFFUSELIGHTING( bool i )
	{
		m_nDIFFUSELIGHTING = i ? 1 : 0;
#ifdef _DEBUG
		m_bDIFFUSELIGHTING = true;
#endif
	}
private:
	int m_nHALFLAMBERT;
#ifdef _DEBUG
	bool m_bHALFLAMBERT;
#endif
public:
	void SetHALFLAMBERT( int i )
	{
		Assert( i >= 0 && i <= 1 );
		m_nHALFLAMBERT = i;
#ifdef _DEBUG
		m_bHALFLAMBERT = true;
#endif
	}
	void SetHALFLAMBERT( bool i )
	{
		m_nHALFLAMBERT = i ? 1 : 0;
#ifdef _DEBUG
		m_bHALFLAMBERT = true;
#endif
	}
public:
	vertexlit_lighting_only_ps30_Static_Index( )
	{
#ifdef _DEBUG
		m_bDIFFUSELIGHTING = false;
#endif // _DEBUG
		m_nDIFFUSELIGHTING = 0;
#ifdef _DEBUG
		m_bHALFLAMBERT = false;
#endif // _DEBUG
		m_nHALFLAMBERT = 0;
	}
	int GetIndex()
	{
		// Asserts to make sure that we aren't using any skipped combinations.
		// Asserts to make sure that we are setting all of the combination vars.
#ifdef _DEBUG
		bool bAllStaticVarsDefined = m_bDIFFUSELIGHTING && m_bHALFLAMBERT;
		Assert( bAllStaticVarsDefined );
#endif // _DEBUG
		return ( 2 * m_nDIFFUSELIGHTING ) + ( 4 * m_nHALFLAMBERT ) + 0;
	}
};
#define shaderStaticTest_vertexlit_lighting_only_ps30 psh_forgot_to_set_static_DIFFUSELIGHTING + psh_forgot_to_set_static_HALFLAMBERT + 0
class vertexlit_lighting_only_ps30_Dynamic_Index
{
private:
	int m_nAMBIENT_LIGHT;
#ifdef _DEBUG
	bool m_bAMBIENT_LIGHT;
#endif
public:
	void SetAMBIENT_LIGHT( int i )
	{
		Assert( i >= 0 && i <= 1 );
		m_nAMBIENT_LIGHT = i;
#ifdef _DEBUG
		m_bAMBIENT_LIGHT = true;
#endif
	}
	void SetAMBIENT_LIGHT( bool i )
	{
		m_nAMBIENT_LIGHT = i ? 1 : 0;
#ifdef _DEBUG
		m_bAMBIENT_LIGHT = true;
#endif
	}
public:
	vertexlit_lighting_only_ps30_Dynamic_Index()
	{
#ifdef _DEBUG
		m_bAMBIENT_LIGHT = false;
#endif // _DEBUG
		m_nAMBIENT_LIGHT = 0;
	}
	int GetIndex()
	{
		// Asserts to make sure that we aren't using any skipped combinations.
		// Asserts to make sure that we are setting all of the combination vars.
#ifdef _DEBUG
		bool bAllDynamicVarsDefined = m_bAMBIENT_LIGHT;
		Assert( bAllDynamicVarsDefined );
#endif // _DEBUG
		return ( 1 * m_nAMBIENT_LIGHT ) + 0;
	}
};
#define shaderDynamicTest_vertexlit_lighting_only_ps30 psh_forgot_to_set_dynamic_AMBIENT_LIGHT + 0
