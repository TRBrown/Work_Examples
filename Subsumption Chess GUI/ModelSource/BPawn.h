#ifndef BPAWN_H
#define BPAWN_H

#pragma warning(disable:4995)

#include "d3dx9.h" 
#include "d3d9.h"
#include "D3D9.h"
#include <string>
#include "CustomModel.h"
#include "Black.h"

class BPawn : public Black
{
private:

public:

	BPawn(IDirect3DDevice9* pd3dDevice);

	BPawn(const BPawn &);

	~BPawn();

	//virtual void Loadmodel();
	//virtual void Loadfx();
	//virtual void AssignParameters(D3DLIGHT9& light1, D3DLIGHT9& light2, D3DXVECTOR3& eye, D3DMATRIX& view, D3DMATRIX& proj);
	virtual void update();
	//virtual void render();
  
};

#endif