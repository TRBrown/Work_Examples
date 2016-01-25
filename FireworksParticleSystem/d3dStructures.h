#ifndef D3DSTRUCTURES_H
#define D3DSTRUCTURES_H
#pragma warning(disable:4995)

#define D3DFVF_CUSTOMVERTEX_PARTICLE (D3DFVF_XYZ|D3DFVF_DIFFUSE)

#include <d3dx9.h>
#include "d3dUtils.h"
#include <string>

//===============================================================
// Globals for convenient access.

class CameraManager;
extern CameraManager* gCManager;

//===============================================================

enum XMLtypes { XMLFountain, XMLRocket};

struct XMLEvent
{
	int					iStart; 
	std::string			sType; 
	D3DCOLOR			d3cColour; 
	int					iDuration;
	D3DXVECTOR3			v3Position;
	D3DXVECTOR3			v3Velocity;
	bool				bActive;
	int					iEventType;
};

// Set up the vertex structure for the particles
struct VertexParticle
{
	D3DXVECTOR3 psPosition;
	D3DXVECTOR3 initialVelocity;
	float       initialSize; // In pixels.
	float       initialTime;
	float       lifeTime;
	float       mass;
	D3DCOLOR    colour;

	static IDirect3DVertexDeclaration9* Decl;
};



void InitAllVertexDeclarations(IDirect3DDevice9* m_pD3dDevice);
void DestroyAllVertexDeclarations();

#endif