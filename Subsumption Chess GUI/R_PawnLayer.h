#ifndef R_PAWNLAYER_H
#define R_PAWNLAYER_H

#pragma warning(disable:4995)

#include "Layer.h"
#include "BoardManager.h"
#include "ModelSource\Piece.h"
#include <list>
#include <vector>

class R_PawnLayer : public Layer
{
private:



protected:


public:

	R_PawnLayer(Layer* parentPointer);

	R_PawnLayer(const R_PawnLayer &);

	~R_PawnLayer();

	//virtual void AddSubLayer(Layer* subLayer);
	virtual void Execute(BoardManager* pBoardManager);


};

#endif