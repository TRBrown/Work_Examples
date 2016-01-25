#ifndef REACTIVELAYER_H
#define REACTIVELAYER_H

#pragma warning(disable:4995)

#include "Layer.h"
#include "BoardManager.h"
#include "R_PawnLayer.h"
#include "R_KnightLayer.h"
#include <list>

class ReactiveLayer : public Layer
{
private:



protected:


public:

	ReactiveLayer(Layer* parentPointer, int Movnumber);

	ReactiveLayer(const ReactiveLayer &);

	~ReactiveLayer();

	void SetAIMovesFinal(int MovNumb);

	//virtual void AddSubLayer(Layer* subLayer);
	virtual void Execute(BoardManager* pBoardManager);


};

#endif