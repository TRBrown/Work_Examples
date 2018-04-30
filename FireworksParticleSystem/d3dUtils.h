#ifndef D3DUTILS_H
#define D3DUTILS_H
#pragma warning(disable:4995)

// Enable extra D3D debugging in debug builds if using the debug DirectX runtime.  
// This makes D3D objects work well in the debugger watch window, but slows down 
// performance slightly.
#if defined(DEBUG) | defined(_DEBUG)
#ifndef D3D_DEBUG_INFO
#define D3D_DEBUG_INFO
#endif
#endif

#include <d3d9.h>
#include <d3dx9.h>
#include <dxerr.h>
#include <string>
#include <vector>
#include <iostream>
#include <sstream>

const D3DXCOLOR WHITE(1.0f, 1.0f, 1.0f, 1.0f);
const D3DXCOLOR BLACK(0.0f, 0.0f, 0.0f, 1.0f);
const D3DXCOLOR RED(1.0f, 0.0f, 0.0f, 1.0f);
const D3DXCOLOR GREEN(0.0f, 1.0f, 0.0f, 1.0f);
const D3DXCOLOR BLUE(0.0f, 0.0f, 1.0f, 1.0f);

//WString to Char*
void narrow ( char*& in, const std::wstring& str );

//String to WString
std::wstring widenstr( const std::string& str );

//String to WChar_T*
void widen( wchar_t*& in, const std::string& str );

//Char* to Wchar_T*
void charTowchar( wchar_t* in, const char* pChar );

float GetRandomFloat(float a, float b);
void GetRandomVec(D3DXVECTOR3& out);



//Handy Utility for hresult
#if defined(DEBUG) | defined(_DEBUG)
#ifndef HR
#define HR(x)												\
{															\
HRESULT hr = x;											    \
if(FAILED(hr))											    \
{															\
	wchar_t* wc;                                            \
    widen(wc, #x);                                          \
	DXTrace(__FILE__, __LINE__, hr, wc, TRUE);				\
	delete [] wc;											\
}															\
}
#endif

#else
#ifndef HR
#define HR(x) x;
#endif
#endif


#endif // D3DUTILS_H
