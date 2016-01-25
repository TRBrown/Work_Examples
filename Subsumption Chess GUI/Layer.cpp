#include "DXUT.h"
#include "Layer.h"

Layer::Layer()
{


}

Layer::~Layer()
{

}

void Layer::AddSubLayer(Layer* subLayer)
{
		m_pLayers.push_back(subLayer);	
}

void Layer::SetLayerMove(D3DXVECTOR3 layerMove)
{
	m_pParentLayer->AddMove(layerMove);
}

void Layer::AddMove(D3DXVECTOR3 layerMove)
{
	m_vv4Output.push_back(layerMove);
}

void Layer::Execute(BoardManager* pBoardManager)
{
	m_pBoardManager = pBoardManager;
	std::list<Layer*>::iterator i;

	for(i=m_pLayers.begin(); i != m_pLayers.end(); ++i)
	{		
		Layer* x = *i;
		x->Execute(m_pBoardManager);

	};
}

void Layer::Execute(BoardManager* pBoardManager, int MoveNumber)
{
	m_pBoardManager = pBoardManager;
	MoveNum = MoveNumber;
	std::list<Layer*>::iterator i;

	for(i=m_pLayers.begin(); i != m_pLayers.end(); ++i)
	{		
		Layer* x = *i;
		x->Execute(m_pBoardManager);

	};
}
