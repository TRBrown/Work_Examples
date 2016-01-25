#include "DXUT.h" 
#include "d3dStructures.h"
#include <dxerr.h>

// Initialize static variables.
IDirect3DVertexDeclaration9* VertexParticle::Decl  = 0;

void InitAllVertexDeclarations(IDirect3DDevice9* m_pD3dDevice)
{

	//===============================================================
	// Particle

	D3DVERTEXELEMENT9 ParticleElements[] = 
	{
		{0, 0,  D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},
		{0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0},
		{0, 24, D3DDECLTYPE_FLOAT1, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 1},
		{0, 28, D3DDECLTYPE_FLOAT1, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 2},
		{0, 32, D3DDECLTYPE_FLOAT1, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 3},
		{0, 36, D3DDECLTYPE_FLOAT1, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 4},
		{0, 40, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 0},
		D3DDECL_END()
	};	
	HR(m_pD3dDevice->CreateVertexDeclaration(ParticleElements, &VertexParticle::Decl));
}

void DestroyAllVertexDeclarations()
{
	VertexParticle::Decl->Release();
	VertexParticle::Decl  = 0;
}