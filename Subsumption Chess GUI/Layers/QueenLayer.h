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

	BoardManager* m_pBoardManager; // pointer to board manager
	Layer* m_pParentLayer; //parent layer
	std::list<Layer*> m_pLayers; // child layers

protected:

	std::vector<D3DXVECTOR4> m_vv4Output; // Outputted list
	std::vector<int> m_viWeights; //  List of weights

public:

	Layer(BoardManager* pBoardManager, Layer* pParent);

	Layer(const Layer &);

	~Layer();

	virtual void AddSubLayer();
	virtual void Execute();


};

#endif