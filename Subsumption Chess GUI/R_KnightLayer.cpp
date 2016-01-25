#include "DXUT.h"
#include "R_KnightLayer.h"

R_KnightLayer::R_KnightLayer(Layer* parentPointer) : Layer() 
{
		m_pParentLayer = parentPointer;
}

void R_KnightLayer::Execute(BoardManager* pBoardManager)
{
	m_pBoardManager = pBoardManager;
	
	std::vector<Piece*>::iterator i;

	for(i=m_pBoardManager->m_vPieces.begin(); i != m_pBoardManager->m_vPieces.end(); ++i)
	{		
		int type = NULL;
		std::vector<int> movelist;
		Piece* x = *i;
		int colour = x->GetPieceColour();
		if(colour == 1)
		{
			type = x->GetPieceType();
			
			if(type == 4)
			{
				movelist = x->GetMoveList();
				if(!movelist.empty())
				{
					D3DXVECTOR3 mTmp(x->GetBoardPosition(), movelist[0], 2);
					SetLayerMove(mTmp);
					break;
				}
			}
		}

	};

}
