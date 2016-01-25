#ifndef AABBRENDER
#define AABBRENDER
#pragma warning(disable:4995)

#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZ|D3DFVF_DIFFUSE)

#include "d3dx9.h" 
#include "d3d9.h"
#include <string>
#include <malloc.h>
#include "..//Custom_Math.h"


class AABBRender
{
private:


protected:

	struct CUSTOMVERTEX
	{
		D3DXVECTOR3 position;
		//D3DXVECTOR3 normal;
		DWORD       colour;
	};

	LPDIRECT3DVERTEXBUFFER9 m_pVertexBuffer;


public:

	HRESULT					hr;
	IDirect3DDevice9*		m_pD3dDevice;

	
	AABBRender(AABB boundingbox);
	~AABBRender();


	void Loadmodel(AABB boundingbox);
	void render(D3DXMATRIX view, D3DXMATRIX projection);

};




#endif

