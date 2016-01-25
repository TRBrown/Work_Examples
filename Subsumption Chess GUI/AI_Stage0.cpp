#include "DXUT.h"
#include "AI_Stage0.h"
#include "ReactiveLayer.h"

AI_Stage0::AI_Stage0() : Layer() 
{

}

void AI_Stage0::InitStage0() 
{
	ReactiveLayer* Reactive = new ReactiveLayer(this, MoveNum);
	AddSubLayer(Reactive);
}

AI_Stage0::~AI_Stage0()
{
	std::list<Layer*>::iterator i;

	for(i=m_pLayers.begin(); i != m_pLayers.end(); ++i)
	{		
		delete *i;
	};
}

void AI_Stage0::Execute(BoardManager* pBoardManager, int MovNumb)
{
	m_pBoardManager = pBoardManager;
	MoveNum = MovNumb;
	InitStage0();
	std::list<Layer*>::iterator i;

	for(i=m_pLayers.begin(); i != m_pLayers.end(); ++i)
	{		
		Layer* x = *i;
		x->Execute(m_pBoardManager);

	};
}