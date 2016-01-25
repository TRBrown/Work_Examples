#ifndef LAYER_H
#define LAYER_H

#pragma warning(disable:4995)

#include "d3dx9.h" 
#include "d3d9.h"
#include "D3D9.h"
#include "BoardManager.h"
#include <list>


class Layer
{
private:

protected:

	BoardManager* m_pBoardManager; // pointer to board manager
	Layer* m_pParentLayer; //parent layer
	std::list<Layer*> m_pLayers; // child layers
	std::vector<D3DXVECTOR3> m_vv4Output; // Outputted list
	int MoveNum;

public:

	Layer();

	Layer(const Layer &);

	~Layer();

	virtual void AddSubLayer(Layer* subLayer);
	virtual void AddMove(D3DXVECTOR3 layerMove);//adds to self
	virtual void SetLayerMove(D3DXVECTOR3 layerMove); // adds to parent
	virtual void Execute(BoardManager* pBoardManager);
	virtual void Execute(BoardManager* pBoardManager, int MoveNumber);


};

#endif