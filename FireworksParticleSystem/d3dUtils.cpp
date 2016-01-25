#include "DXUT.h" 
#include "d3dUtils.h"
#include <dxerr.h>

//converts wchar* string to char*
void narrow( char*& in, const std::wstring& str )
{
	std::ostringstream stm ;
	const std::ctype<char>& ctfacet =
		std::use_facet< std::ctype<char> >( stm.getloc() ) ;
	for( size_t i=0 ; i<str.size() ; ++i )
		stm << ctfacet.narrow( str[i], 0 ) ;
	std::string str1 = stm.str();
	in = new char [str.size()+1];
	strcpy(in, str1.c_str());
}

//Converts char* string to Wchar string
std::wstring widenstr( const std::string& str )
{
	std::wostringstream wstm ;
	const std::ctype<wchar_t>& ctfacet = 
		std::use_facet< std::ctype<wchar_t> >( wstm.getloc() ) ;
	for( size_t i=0 ; i<str.size() ; ++i ) 
		wstm << ctfacet.widen( str[i] ) ;
	return wstm.str() ;
}

//Converts char* string to Wchar* array
void widen( wchar_t*& in, const std::string& str )
{
	std::wostringstream wstm ;
	const std::ctype<wchar_t>& ctfacet = 
		std::use_facet< std::ctype<wchar_t> >( wstm.getloc() ) ;
	for( size_t i=0 ; i<str.size() ; ++i ) 
		wstm << ctfacet.widen( str[i] ) ;
	std::wstring wstr = wstm.str();
	in = new wchar_t [str.size()+1];
	wcscpy(in, wstr.c_str());
}

//Converts char* to Wchar* 
void charTowchar( wchar_t* in, const char* pChar )
{
	
	int size = MultiByteToWideChar(CP_ACP, 0, pChar, -1, 0, 0);
	in = (wchar_t*)malloc(size * sizeof(wchar_t));
	MultiByteToWideChar(CP_ACP, 0, pChar, -1, in, size);

}



float GetRandomFloat(float a, float b)
{
	if( a >= b ) // bad input
		return a;

	// Get random float in [0, 1] interval.
	float f = (rand()%10001) * 0.0001f;

	return (f*(b-a))+a;
}

void GetRandomVec(D3DXVECTOR3& out)
{
	out.x = GetRandomFloat(-1.0f, 1.0f);
	out.y = GetRandomFloat(-1.0f, 1.0f);
	out.z = GetRandomFloat(-1.0f, 1.0f);

	// Project onto unit sphere.
	D3DXVec3Normalize(&out, &out);
}