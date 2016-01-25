#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H
#pragma warning(disable:4995)

#include "d3dx9.h" 
#include "d3d9.h"
#include "CustomModel.h"
#include "CollisionManager.h"
#include "CameraManager.h"
#include "FontManager.h"
#include "Custom_Math.h"
#include <list>
//#include <string>
//#include <vector>

class InputManager
{
private:
	
	FontManager* m_pFmanager;
	CameraManager* m_pCmanager;
	CollisionManager* m_pCollmanager;
	
	std::list<CustomModel*>* m_lModels;
	D3DXVECTOR4* m_pForcePosition;

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

	void Initialise(FontManager* Fmanager, CameraManager* CManager, 
					CollisionManager* CollManager, std::list<CustomModel*>& models,
					D3DXVECTOR4& forcePosition);

	void CatchKeyboardDown(UINT nChar, bool bKeyDown, bool bAltDown);

	void CatchMouse ( bool bLeftButtonDown, bool bRightButtonDown, bool bMiddleButtonDown,
					  bool bSideButton1Down, bool bSideButton2Down, int nMouseWheelDelta,
				      int xPos, int yPos );
	
	void MouseWheel ();
	void MouseButtons ();
				  
	//Accessors


};




#endif

