#include "DXUT.h"
#include "ReactiveLayer.h"

ReactiveLayer::ReactiveLayer(Layer* parentPointer, int MovNumber) : Layer() 
{
	MoveNum = MovNumber;
	m_pParentLayer = parentPointer;
	R_PawnLayer* Pawn = new R_PawnLayer(this);
	AddSubLayer(Pawn);
	R_KnightLayer* Knight = new R_KnightLayer(this);
	AddSubLayer(Knight);

}

ReactiveLayer::~ReactiveLayer()
{
	std::list<Layer*>::iterator i;

	for(i=m_pLayers.begin(); i != m_pLayers.end(); ++i)
	{		
		delete *i;
	};
}

void ReactiveLayer::SetAIMovesFinal(int MovNumb)
{
	std::vector<D3DXVECTOR3>::iterator i;

	for(i=m_vv4Output.begin(); i != m_vv4Output.end(); ++i)
	{		
		D3DXVECTOR3 x = *i;
		m_pBoardManager->InsertAIMove(MoveNum, x.x, x.y, x.z);

	};
	
}

void ReactiveLayer::Execute(BoardManager* pBoardManager)
{
	m_pBoardManager = pBoardManager;
	std::list<Layer*>::iterator i;

	for(i=m_pLayers.begin(); i != m_pLayers.end(); ++i)
	{		
		Layer* x = *i;
		x->Execute(m_pBoardManager);

	};
	SetAIMovesFinal(MoveNum);
}