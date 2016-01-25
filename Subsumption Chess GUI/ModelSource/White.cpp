#include "DXUT.h" 
#include "White.h"
#include "CustomModel.h"

White::White(IDirect3DDevice9* pd3dDevice) : Piece(pd3dDevice)
{
	m_iPieceColour = 0;
}

White::~White()

{

}