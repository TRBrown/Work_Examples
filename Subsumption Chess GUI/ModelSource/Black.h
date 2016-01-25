#ifndef BLACK
#define BLACK

#pragma warning(disable:4995)

#include <string>
#include "Piece.h"

class Black : public Piece
{
private:

protected:


public:

	Black(IDirect3DDevice9* pd3dDevice);

	Black(const Black&);

	~Black();

};

#endif