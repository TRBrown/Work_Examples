#ifndef PIECE
#define PIECE

#pragma warning(disable:4995)

#include <string>
#include <vector>
#include "CustomModel.h"

class Piece : public CustomModel
{
private:

protected:
	
	int m_iPieceColour; // 0 = White , 1 = Black
	int m_iPieceType; //Enumerated int of piece type
	bool m_bslides; //Slides or hops
	UINT m_uiBoardPosition; // The position on the 0x88 board
	std::vector<int> m_vMoveList;


public:

	D3DXVECTOR3 m_v3Moveto;
	
	Piece(IDirect3DDevice9* pd3dDevice);

	//Piece(const Piece &);

	~Piece();

	void			Effect_Select_Reset();
	void			Effect_Select_Highlight();
	void			Effect_Select_Selected();
	void			Effect_Select_SelectedMoves();

	void			PushMoveList(const INT val){m_vMoveList.push_back(val);}
	void			ClearMoveList(){m_vMoveList.clear();}
	std::vector<int>	GetMoveList() const{return m_vMoveList;}

	void			SetBoardPosition(const UINT val)	{m_uiBoardPosition = val;}
	UINT			GetBoardPosition() const	{return m_uiBoardPosition;}
	INT				GetPieceType() const	{return m_iPieceType;}
	INT				GetPieceColour() const	{return m_iPieceColour;}
	BOOL			GetSlidingType() const	{return m_bslides;}

	virtual void	render(D3DXVECTOR3 v3Position);
};

#endif