#ifndef AI_STAGE0_H
#define AI_STAGE0_H

#pragma warning(disable:4995)

#include "Layer.h"
#include "BoardManager.h"
#include <list>

class AI_Stage0 : public Layer
{
private:



protected:


public:

	AI_Stage0();

	AI_Stage0(const AI_Stage0 &);

	~AI_Stage0();

	void InitStage0();
	//virtual void AddSubLayer(Layer* subLayer);
	virtual void Execute(BoardManager* pBoardManager, int MovNumb);


};

#endif