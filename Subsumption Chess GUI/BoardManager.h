#ifndef BOARDMANAGER_H
#define BOARDMANAGER_H

#pragma warning(disable:4995)

#include "d3dx9.h" 
#include "d3d9.h"
//#include "D3D9.h"
#include <string>
#include <list>
#include <vector>
#include <algorithm>
#include "Models.h"
#include "Custom_Services.h"
#include "Custom_Math.h"

struct BoardPosition 
{
	D3DXVECTOR4 position;
	AABB positionArea;
	AABBRender* areaRender;
};

class BoardManager
{
private:

	Piece* m_paBoard[128]; // Actual board
	
	BoardPosition m_v4BoardPositions[128];//3d Positions
	
	std::list<CustomModel*>* p_lModels; // List of Models
	std::list<Piece*>* p_lPieces;
		

	// 2d vector vector<vector> of outputted moves of ([movenumber][move])
	//Records the highest level of subsumption or lowest rather (level 0)
	//Movenumber represents the AI moves not the current board move
	//Format x=source, y=destination, z = weight
	std::vector< std::vector<D3DXVECTOR3> > m_vv4Output; 
	
	int movenumber; //Current Movement number
	
	std::list<D3DXVECTOR4> m_v4MoveHistory; // Move History of game

	UINT m_uiPlayerColour; // Not used yet
	UINT m_uiMoveTimer; // Not used yet

	//Movement variables
	static enum m_ePeiceType;
	static enum m_eCollisionReturn;
	static int m_iaWPawnStart[8];
	static int m_iaBPawnStart[8];
	static int m_iaKing[9];
	static int m_iaQueen[9];
	static int m_iaRook[5];
	static int m_iaBishop[5];
	static int m_iaKnight[9];
	static int m_iaWPawn[5];
	static int m_iaBPawn[5];
	static int m_iaMovementNumber[7];

	IDirect3DDevice9* pd3dDevice;
	DWORD   m_dwThreadId;
	Custom_Services* m_pManagers;

	BoardManager();

	BoardManager(const BoardManager &);

public:

	int m_iAIMovenumber;

	std::vector<Piece*> m_vPieces; // Vector of Pieces
	std::list<AABBRender*> p_lPositionAABBs; //List of bounding box's for render

	static BoardManager* Instance(std::list<CustomModel*>& g_lModels, 
									std::list<Piece*>& g_lPieces, 
									IDirect3DDevice9* pd3dDevicer, 
									Custom_Services* Managers);

	~BoardManager();

	void InitGenerateMoves();
	void GenerateMoves(Piece* pieceTmp, int square, int* movedelta, int movemetric);
	bool GenerateMove(Piece* pieceTmp, int sourceSquare, int index);
	void MakeMove(D3DXVECTOR4 move);// For player Move format
	void MakeAIMove(D3DXVECTOR3 move);// 
	bool UserMoveCheck(D3DXVECTOR4 move); // returns true for bad move or false for OK

	void SetModels();
	void SetPositions();
	void SetPiece(Piece* Pieice, int Location);

	void UpdateMoveHistory(const D3DXVECTOR4 val){m_v4MoveHistory.push_back(val);}
	int DetectCollision(int colour, int indexDest);
	

	//AI Move stuff
	// Change when a additive scoring of outputs is required 
	D3DXVECTOR3		GetAIMove();
	void		InsertAIMove(int MoveNum, int source, int destination, int weight);
	void		IncrementMoveNumber();
	void 		AddAIMoveRow(){m_vv4Output.push_back(std::vector<D3DXVECTOR3>());};
	void		SignalAIFinished(DWORD dwThreadId);
	void		SetActiveThreadIdent(const DWORD   ThreadId){m_dwThreadId = ThreadId;}
//	std::vector<Piece*>* GetPieces(){return &m_vPieces;}

	UINT			GetMoveTimer()const{return m_uiMoveTimer;}
	void			SetMoveTimer(const UINT time){m_uiMoveTimer = time;}

	//Board Position Stuff
	UINT			GetPieceColour()const{return m_uiPlayerColour;}
	Piece**			GetBoard(){return m_paBoard;}
	CustomModel*	GetPeice(int Position)const{return m_paBoard[Position];}
	D3DXVECTOR4		GetPosition3D(int Position)const{return m_v4BoardPositions[Position].position;}
	AABB			GetPositionAABB(int Position)const{return m_v4BoardPositions[Position].positionArea;}


};

#endif