#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H
#pragma warning(disable:4995)

#include "d3dx9.h" 
#include "d3d9.h"
#include "CameraManager.h"
#include "FontManager.h"
#include "Custom_Math.h"
#include "Custom_Services.h"
#include "CollisionManager.h"
#include "ModelSource\Piece.h"
//#include <string>
//#include <vector>


class InputManager
{
private:
	
	FontManager* m_pFmanager;
	CameraManager* m_pCmanager;
	Custom_Services* m_pServices;
	CollisionManager* m_pCollmanager;
	BoardManager* m_pBoardmanager;
	std::list<Piece*>* m_lPieces; // List of piece Models
	std::list<CustomModel*>* m_lModels; // List of piece Models
	std::list<Piece*>* m_lMoveModels; // List of piece Models
	
	Piece* m_pHighlighted;
	Piece* m_pSelected;
	Piece* m_pMove;
	
	bool m_bAltDown;

	bool m_bLeftButtonDown;
	bool m_bRightButtonDown;
	bool m_bMiddleButtonDown;
	bool m_bSideButton1Down;
	bool m_bSideButton2Down;
	int m_iMouseWheelDelta;
	int m_ixPos;
	int m_iyPos;

	bool m_bLRButtonHold;
	int m_ixPosPrevious;
	int m_iyPosPrevious;
	
	InputManager();

	InputManager(const InputManager &);

protected:


public:

	static InputManager* Instance();

	~InputManager();

	void Initialise(Custom_Services* GameManagers, 
					std::list<Piece*>& g_lPieces, 
					std::list<CustomModel*>& g_lModels);

	void CatchKeyboardDown(UINT nChar, bool bKeyDown, bool bAltDown);

	void CatchMouse ( bool bLeftButtonDown, bool bRightButtonDown, bool bMiddleButtonDown,
					  bool bSideButton1Down, bool bSideButton2Down, int nMouseWheelDelta,
				      int xPos, int yPos );
	
	void MouseWheel ();
	void MouseButtons ();
	void MouseOver ();

	void PickRay(int xPos, int yPos, D3DXVECTOR3 &rayOrg, D3DXVECTOR3 &rayDelta);				  
	//Accessors


};




#endif

