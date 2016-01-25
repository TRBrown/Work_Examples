#ifndef R_KNIGHTLAYER_H
#define R_KNIGHTLAYER_H

#pragma warning(disable:4995)

#include "Layer.h"
#include "BoardManager.h"
#include "ModelSource\Piece.h"
#include <list>
#include <vector>

class R_KnightLayer : public Layer
{
private:



protected:


public:

	R_KnightLayer(Layer* parentPointer);

	R_KnightLayer(const R_KnightLayer &);

	~R_KnightLayer();

	//virtual void AddSubLayer(Layer* subLayer);
	virtual void Execute(BoardManager* pBoardManager);


};

#endif