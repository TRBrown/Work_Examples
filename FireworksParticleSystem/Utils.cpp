#include "DXUT.h" 
#include "d3dUtils.h"
#include <dxerr.h>
#pragma warning(disable:4995)

//converts wchar* string to char*
char* narrow( const std::wstring& str )
{
	std::ostringstream stm ;
	const std::ctype<char>& ctfacet =
		std::use_facet< std::ctype<char> >( stm.getloc() ) ;
	for( size_t i=0 ; i<str.size() ; ++i )
		stm << ctfacet.narrow( str[i], 0 ) ;
	std::string str1 = stm.str();
	char* c = new char [str.size()+1];
	strcpy(c, str1.c_str());
	return c;
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
wchar_t* widen( const std::string& str )
{
	std::wostringstream wstm ;
	const std::ctype<wchar_t>& ctfacet = 
		std::use_facet< std::ctype<wchar_t> >( wstm.getloc() ) ;
	for( size_t i=0 ; i<str.size() ; ++i ) 
		wstm << ctfacet.widen( str[i] ) ;
	std::wstring wstr = wstm.str();
	wchar_t* wc = new wchar_t [str.size()+1];
	wcscpy(wc, wstr.c_str());
	return wc ;
}