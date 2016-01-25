#include "DXUT.h" 
#include "BoardManager.h"
enum BoardManager::m_ePeiceType { King, Queen, Rook, Bishop, Knight, W_Pawn, B_Pawn };
enum BoardManager::m_eCollisionReturn { Blocked, Take, SClear };
int BoardManager::m_iaWPawnStart[8] = { 23, 22, 21, 20, 19, 18, 17, 16};
int BoardManager::m_iaBPawnStart[8] = { 103, 102, 101, 100, 99, 98, 97, 96};
int BoardManager::m_iaKing[9] = { 17, 16, 15, 1, -1, -17, 16 -15, 0};
int BoardManager::m_iaQueen[9] = { 17, 16, 15, 1, -1, -17, 16 -15, 0};
int BoardManager::m_iaRook[5] = { 16, 1, -1, 16, 0};
int BoardManager::m_iaBishop[5] = { 17, 15, -17, -15, 0};
int BoardManager::m_iaKnight[9] = { 33, 31, 18, 14, -14, -18, -31, -33, 0};
int BoardManager::m_iaWPawn[5] = { 32, 17, 16, 15, 0};
int BoardManager::m_iaBPawn[5] = { -32 ,-17, -16, -15, 0};
int BoardManager::m_iaMovementNumber[7] = { 1, 7, 7, 7, 1, 2, 2};

BoardManager::BoardManager()
{
	SetPositions();
	//*Note* Good practice set pointers to Null first
	for (int x = 0; x < 128; x++)
	{
		m_paBoard[x] = NULL;
	}

	m_iAIMovenumber = 0;
	movenumber = 1;
	m_uiMoveTimer = 1;
	m_uiPlayerColour = 0;

};


BoardManager* BoardManager::Instance(std::list<CustomModel*>& g_lModels, 
									std::list<Piece*>& g_lPieces, 
									IDirect3DDevice9* t_pd3dDevice,
									Custom_Services* Managers)
{
	static BoardManager instance;
	instance.p_lModels = &g_lModels;
	instance.p_lPieces = &g_lPieces;
	instance.pd3dDevice = t_pd3dDevice;
	instance.m_pManagers = Managers;
	instance.SetModels();
	return &instance;

}

BoardManager::~BoardManager()
{
	std::list<AABBRender*>::iterator i;

	for(i=p_lPositionAABBs.begin(); i != p_lPositionAABBs.end(); ++i)
	{		
		delete *i;
	};
}

/*CustomModel::CustomModel(const CustomModel & rhs)
{
	m_sMeshFN		=		rhs.m_sMeshFN;
	m_sFXFN			=		rhs.m_sFXFN;
	m_pD3dDevice	=		rhs.m_pD3dDevice;
	m_bError		=		false;

	Loadmodel();
	Loadfx();

	m_pLight1		=		rhs.m_pLight1;
	m_v4Ambient		=		rhs.m_v4Ambient;
	m_mScale		=		rhs.m_mScale;
	m_mRotationX	=		rhs.m_mRotationX;
	m_mRotationY	=		rhs.m_mRotationY;
	m_mRotationZ	=		rhs.m_mRotationZ;
	m_mTranslation	=		rhs.m_mTranslation;
	m_mWorld		=		rhs.m_mWorld;
	m_mView			=		rhs.m_mView;
	m_mProjection	=		rhs.m_mProjection;

}*/

//Instantiates Models and places
void BoardManager::SetModels()
{

	Chessboard* chessboard = new Chessboard(pd3dDevice);
	p_lModels->push_back(chessboard);

#pragma region White Pawns
	//White pawns
	WPawn* wpawn1 = new WPawn(pd3dDevice);
	SetPiece(wpawn1, 16);	
	WPawn* wpawn2 = new WPawn(pd3dDevice);
	SetPiece(wpawn2, 17);
	WPawn* wpawn3 = new WPawn(pd3dDevice);
	SetPiece(wpawn3, 18);
	WPawn* wpawn4 = new WPawn(pd3dDevice);
	SetPiece(wpawn4, 19);
	WPawn* wpawn5 = new WPawn(pd3dDevice);
	SetPiece(wpawn5, 20);
	WPawn* wpawn6 = new WPawn(pd3dDevice);
	SetPiece(wpawn6, 21);
	WPawn* wpawn7 = new WPawn(pd3dDevice);
	SetPiece(wpawn7, 22);
	WPawn* wpawn8 = new WPawn(pd3dDevice);
	SetPiece(wpawn8, 23);
#pragma endregion White Pawns

#pragma region White Main Pieces
	//White Main Pieces
	WRook* wrook1 = new WRook(pd3dDevice);
	SetPiece(wrook1, 0);	
	WRook* wrook2 = new WRook(pd3dDevice);
	SetPiece(wrook2, 7);
	WKnight* wknight1 = new WKnight(pd3dDevice);
	SetPiece(wknight1, 1);
	WKnight* wknight2 = new WKnight(pd3dDevice);
	SetPiece(wknight2, 6);
	WBishop* wbishop1 = new WBishop(pd3dDevice);
	SetPiece(wbishop1, 2);
	WBishop* wbishop2 = new WBishop(pd3dDevice);
	SetPiece(wbishop2, 5);
	WQueen* wqueen1 = new WQueen(pd3dDevice);
	SetPiece(wqueen1, 3);
	WKing* wking1 = new WKing(pd3dDevice);
	SetPiece(wking1, 4);
#pragma endregion White Main Pieces

#pragma region Black Pawns
	//Black pawns
	BPawn* bpawn1 = new BPawn(pd3dDevice);
	SetPiece(bpawn1, 96);	
	BPawn* bpawn2 = new BPawn(pd3dDevice);
	SetPiece(bpawn2, 97);
	BPawn* bpawn3 = new BPawn(pd3dDevice);
	SetPiece(bpawn3, 98);
	BPawn* bpawn4 = new BPawn(pd3dDevice);
	SetPiece(bpawn4, 99);
	BPawn* bpawn5 = new BPawn(pd3dDevice);
	SetPiece(bpawn5, 100);
	BPawn* bpawn6 = new BPawn(pd3dDevice);
	SetPiece(bpawn6, 101);
	BPawn* bpawn7 = new BPawn(pd3dDevice);
	SetPiece(bpawn7, 102);
	BPawn* bpawn8 = new BPawn(pd3dDevice);
	SetPiece(bpawn8, 103);
#pragma endregion Black Pawns

#pragma region Black Main Pieces
	//Black Main Pieces
	BRook* brook1 = new BRook(pd3dDevice);
	SetPiece(brook1, 119);	
	BRook* brook2 = new BRook(pd3dDevice);
	SetPiece(brook2, 112);
	BKnight* bknight1 = new BKnight(pd3dDevice);
	SetPiece(bknight1, 118);
	BKnight* bknight2 = new BKnight(pd3dDevice);
	SetPiece(bknight2, 113);
	BBishop* bbishop1 = new BBishop(pd3dDevice);
	SetPiece(bbishop1, 117);
	BBishop* bbishop2 = new BBishop(pd3dDevice);
	SetPiece(bbishop2, 114);
	BQueen* bqueen1 = new BQueen(pd3dDevice);
	SetPiece(bqueen1, 115);
	BKing* bking1 = new BKing(pd3dDevice);
	SetPiece(bking1, 116);
#pragma endregion Black Main Pieces

}

//Generate a array of the board positions
void BoardManager::SetPositions()
{
	int x;
	int i;
	float xInc = 112;
	for(x=0;x<8;x++)
	{
		
		int arrayInc = x;
		float zInc = 112;
		
		for(i=0;(i<128);i+=16)
		{
			m_v4BoardPositions[arrayInc].position = D3DXVECTOR4(xInc, 1.0f, zInc, 1.0f);
			m_v4BoardPositions[arrayInc].positionArea = AABB(	D3DXVECTOR3((xInc - 10), -20.0f, (zInc - 10)), 
																D3DXVECTOR3((xInc + 10), -10.0f, (zInc + 10)));

			m_v4BoardPositions[arrayInc].areaRender = new AABBRender(m_v4BoardPositions[arrayInc].positionArea);
			p_lPositionAABBs.push_back(m_v4BoardPositions[arrayInc].areaRender);
			arrayInc += 16;
			zInc -= 32;
		}
		xInc -= 32;
	}


}

//Sets a piece on the board
void BoardManager::SetPiece(Piece* Piece, int Location)
{
	m_paBoard[Location] = Piece;
	Piece->SetPosition(m_v4BoardPositions[Location].position); // Sets world position for rendering NOT board
	Piece->SetBoardPosition(UINT(Location));
	p_lModels->push_back(Piece);
	p_lPieces->push_back(Piece);
	m_vPieces.push_back(Piece);
}

//Makes a Player Move
void BoardManager::MakeMove(D3DXVECTOR4 move)
{
	int sIndex = move.y * 16 + move.x;
	int dIndex = move.w * 16 + move.z;
	m_paBoard[sIndex]->SetBoardPosition(dIndex);
	m_paBoard[dIndex] = m_paBoard[sIndex];
	m_paBoard[dIndex]->SetPosition(GetPosition3D(dIndex));
	m_paBoard[sIndex] = NULL;
	UpdateMoveHistory(move);
}

//Makes a AI Move
void BoardManager::MakeAIMove(D3DXVECTOR3 move)
{
	UINT sIndex = (UINT)move.x;
	UINT dIndex = (UINT)move.y;
	m_paBoard[sIndex]->SetBoardPosition(dIndex);
	m_paBoard[dIndex] = m_paBoard[sIndex];
	m_paBoard[dIndex]->SetPosition(GetPosition3D(dIndex));
	m_paBoard[sIndex] = NULL;
	//***** Add move history update here, need vector 4 move format lookup
}

//Initiate Move list generation
void BoardManager::InitGenerateMoves()
{
	std::vector<Piece*>::iterator i;
	int piecetype;

	for(i=m_vPieces.begin(); i != m_vPieces.end(); ++i)
	{		
		Piece* x = *i;
		piecetype = x->GetPieceType();
		int position = x->GetBoardPosition();
		x->ClearMoveList();
		switch ( piecetype )
		{
		case King:
			GenerateMoves(x, position, m_iaKing, 1);
			break;
		case Queen:
			GenerateMoves(x, position, m_iaQueen, 7);
			break;
		case Rook:
			GenerateMoves(x, position, m_iaRook, 7);
			break;
		case Bishop:
			GenerateMoves(x, position, m_iaBishop, 7);
			break;
		case Knight:
			GenerateMoves(x, position, m_iaKnight, 1);
			break;
		case W_Pawn:
			GenerateMoves(x, position, m_iaWPawn, 1);
			break;
		case B_Pawn:
			GenerateMoves(x, position, m_iaBPawn, 1);
			break;
		}
	};
}


//Generates moves for a chess piece (Calls generate move)
void BoardManager::GenerateMoves(Piece* pieceTmp, int square, int * movedelta, int movemetric)
{

	for (; *movedelta; movedelta++) 
	{
		if(*movedelta == 0)break;
		int piecetype = pieceTmp->GetPieceType();
		int index;
		bool doubleMove =  FALSE;
		int	metricCount = 0;
		// double move conditions for pawns
		if(piecetype == 5)
		{
			int* tmpArray = m_iaWPawnStart;
			for (; *tmpArray; tmpArray++)
			{
				if(square == *tmpArray)doubleMove =  true;//Skip over double move delta
			}
		}
		else if (piecetype == 6)
		{
			int* tmpArray = m_iaBPawnStart;
			for (; *tmpArray; tmpArray++) 
			{
				if(square == *tmpArray)doubleMove =  true;//Skip over double move delta
			}		
		}
		//end double move conditions

		for (index = square + *movedelta; !(index & 0x88); (index += *movedelta))
		{
			if(metricCount == movemetric)break;// stops single move generations past 1
			metricCount++;
			
			//Messy logic to catch pawn double move
			if((piecetype >= 5) && ((*movedelta == 32) || (*movedelta == -32)) && !doubleMove)
				continue;
			
			bool temp = GenerateMove(pieceTmp, square, index);
			if (temp)break; // Breaks if a block is found to avoid generating behind it
		}
	}

}

//generates a single Move for a chess piece
//Contains Conditions for certain chess piece colours and types
bool BoardManager::GenerateMove(Piece* pieceTmp, int sourceSquare, int index)
{
	int colour = pieceTmp->GetPieceColour();
	int detect = DetectCollision(colour, index); // 0 = block 1 = take 2 = clear
	
	if(detect)
	{
		//Conditions for pawns 
		//White
		if (pieceTmp->GetPieceType() == 5)
		{

#pragma region PawnMoveWhite
			//*Double Move* White
			if((index - sourceSquare) == 32)//if hostile piece is there
			{
				if (detect == 1)
				{
					return true;//return stop generation
				}
				else//if clear check slide tile
				{
					detect = DetectCollision(colour, (index - 16));//Back one square
					if (detect != 2)return true;//return stop generation *blocked*
					else 
					{
						pieceTmp->PushMoveList(index);
						return false; //Double Move valid
					}
				}
			}
			else if((index - sourceSquare) != 16)// If not a move forward aka a take diagonal
			{
				if(detect == 1)
				{
					pieceTmp->PushMoveList(index);
					return false; //Double Move valid
				}
				else
				{
					return true;//return stop generation *cant move diagonally*
				}
			}
			else
			{
				pieceTmp->PushMoveList(index);
				return false; //if clear, keep going
			}
		}
#pragma endregion PawnMoveWhite
#pragma region PawnMoveBlack
		//Black
		else if (pieceTmp->GetPieceType() == 6)
		{
			//*Double Move* Black
			if((index - sourceSquare) == -32)//if hostile piece is there
			{
				if (detect == 1)
				{
					return true;//return stop generation
				}
				else//if clear check slide tile
				{
					detect = DetectCollision(colour, (index + 16));//Back one square
					if (detect != 2)return true;//return stop generation *blocked*
					else 
					{
						pieceTmp->PushMoveList(index);
						return false; //Double Move valid
					}
				}
			}
			else if((index - sourceSquare) != -16)// If not a move forward aka a take diagonal
			{
				if(detect == 1)
				{
					pieceTmp->PushMoveList(index);
					return false; //Double Move valid
				}
				else
				{
					return true;//return stop generation *cant move diagonally*
				}
			}
			else
			{
				pieceTmp->PushMoveList(index);
				return false; //if clear, keep going
			}
		}
#pragma endregion PawnMoveBlack
		//Everything else
		else
		{
			pieceTmp->PushMoveList(index);
			if (detect == 1) {return true;}// if take, stop generation
			else {return false;} //if clear, keep going
		}
	}

	else
	{
		if (pieceTmp->GetSlidingType()) return true; // true  block 
	}
}

//checks for piece blocks or pieice take events
int BoardManager::DetectCollision(int colour, int indexDest)

{
	if (m_paBoard[indexDest] != NULL)
	{
		if (m_paBoard[indexDest]->GetPieceColour() == colour)
		{
			return Blocked;
		}
		else
		{
			return Take;
		}
	}
	else
	{
		return SClear;
	}
}

//Checks users move is valid
bool BoardManager::UserMoveCheck(D3DXVECTOR4 move)

{
	//calculate index's
	int sIndex = move.y * 16 + move.x;
	int dIndex = move.w * 16 + move.z;
	int dMinusS = dIndex - sIndex;

	int rank = move.w - move.y;
	int file = move.z - move.x;
	int maxExp = max(rank, file); // Squares to be moved
	int movementMetric = dMinusS / maxExp;//get the increment for the slide

	//get colour
	int colour = m_paBoard[sIndex]->GetPieceColour();
	int pieicetype = m_paBoard[sIndex]->GetPieceType();
	bool slidingType = m_paBoard[sIndex]->GetSlidingType();

	if (slidingType)
	{
		int test = sIndex;
		for (int x=0; x!= maxExp; x++)
		{
			if(m_iaMovementNumber[pieicetype] == x)return TRUE;
			test += movementMetric;
			if(!DetectCollision(colour, test))return TRUE;
		}
		return FALSE;
	}
	else
	{
		if(DetectCollision(colour, dIndex))
		{
			return TRUE;
		}
		else
		{
			return TRUE;
		}
	}

 
}

//gets the AI's most valued move
D3DXVECTOR3	BoardManager::GetAIMove()
{

	D3DXVECTOR3 tmpAImove(0,0,0);
	std::vector<D3DXVECTOR3>::iterator  i;

	for(i = m_vv4Output[(m_iAIMovenumber - 1)].begin(); i != m_vv4Output[(m_iAIMovenumber - 1)].end(); ++i)
	{		
		D3DXVECTOR3 testMove = *i;
		if(testMove.z > tmpAImove.z)
		{
			tmpAImove.x = testMove.x;
			tmpAImove.y = testMove.y;
			tmpAImove.z = testMove.z;
		}
	}
	return tmpAImove;
}

//Inserts a AI's move into its move history
void	BoardManager::InsertAIMove(int MoveNum, int source, int destination, int weight)
{
	D3DXVECTOR3 tmp = D3DXVECTOR3(source,destination,weight);
	m_vv4Output[MoveNum].push_back(tmp);
}

//Sets the AI as finished
void	BoardManager::SignalAIFinished(DWORD dwThreadId)
{
	if (dwThreadId == m_dwThreadId){m_uiMoveTimer = 0;}
}

//Increments the games move number
void	BoardManager::IncrementMoveNumber()
{
	m_iAIMovenumber++;
}