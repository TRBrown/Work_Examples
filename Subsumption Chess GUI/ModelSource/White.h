#ifndef WHITE
#define WHITE

#pragma warning(disable:4995)

#include <string>
#include "Piece.h"

class White : public Piece
{
private:

protected:

public:

	White(IDirect3DDevice9* pd3dDevice);

	White(const White&);

	~White();

};

#endif