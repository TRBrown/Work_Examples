#include "DXUT.h" 
#include "Black.h"
#include "Piece.h"

Black::Black(IDirect3DDevice9* pd3dDevice) : Piece(pd3dDevice)
{
	m_iPieceType = 1;
	m_iPieceColour = 1;
}

Black::~Black()

{

}