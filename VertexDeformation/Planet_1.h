#ifndef PLANET1_H
#define PLANET1_H

#pragma warning(disable:4995)

#include "d3dx9.h" 
#include "d3d9.h"
#include "D3D9.h"
#include <string>
#include "CustomModel.h"

class Planet1 : public CustomModel
{
private:

	D3DXVECTOR4*		m_pForcePosition;
	float*				m_pForceRadius;
	float*				m_pForceStrength;
	float				m_fRotationCounter;


public:

	Planet1(IDirect3DDevice9* pd3dDevice);
	Planet1(IDirect3DDevice9* pd3dDevice, D3DXVECTOR4& forcePosition, float& forceRadius, float& forceStrength);

	Planet1(const Planet1 &);

	~Planet1();

	//virtual void Loadmodel();
	//virtual void Loadfx();
	virtual void AssignParameters(D3DLIGHT9& light1, D3DLIGHT9& light2, const D3DXVECTOR3 *eye, const D3DMATRIX *view, const D3DMATRIX *proj);
	virtual void update();
	//virtual void render();
  
};

#endif