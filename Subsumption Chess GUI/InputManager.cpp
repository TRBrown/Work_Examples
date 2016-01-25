#include "DXUT.h" 
#include "InputManager.h"

extern std::list<Piece*> g_lMoveModels;

InputManager::InputManager()
{
	m_pHighlighted = NULL;
	m_pSelected = NULL;
	
	m_bAltDown = false;
	m_bLeftButtonDown = false;
	m_bRightButtonDown = false;
	m_bMiddleButtonDown = false;
	m_bSideButton1Down = false;
	m_bSideButton2Down = false;
	m_iMouseWheelDelta = 0;
	m_ixPos = 0;
	m_iyPos = 0;

	m_bLRButtonHold = false;
	m_ixPosPrevious = 0;
	m_iyPosPrevious = 0;
}

InputManager* InputManager::Instance()
{
	static InputManager instance;
	return &instance;

}

InputManager::~InputManager()

{

}

void InputManager::Initialise(Custom_Services* GameManagers, 
								std::list<Piece*>& g_lPieces, 
								std::list<CustomModel*>& g_lModels)
{
	m_pFmanager = GameManagers->GetService<FontManager>("FontManager");
	m_pCmanager = GameManagers->GetService<CameraManager>("CameraManager");
	m_pCollmanager = GameManagers->GetService<CollisionManager>("CollisionManager");
	m_pBoardmanager = GameManagers->GetService<BoardManager>("BoardManager");
	m_pServices = GameManagers;
	m_lPieces = &g_lPieces;
	m_lModels = &g_lModels;

}

void InputManager::CatchKeyboardDown(UINT nChar, bool bKeyDown, bool bAltDown)
{
	D3DXVECTOR3 viewvector = m_pCmanager->GetViewVector(); //use the x->z and z_x relationship
	viewvector.y = 0;
	D3DXVec3Normalize(&viewvector, &viewvector);
	float XRotation = ((1 * viewvector.z) / 10);
	float ZRotation = -((1 * viewvector.x) / 10);

	//convert to switch when got time
	if ((nChar == 0x26) && m_pCmanager->OrbitAboveCheck()) //up arrow
	{
		Quaternion Qrotation = Quaternion(XRotation, 0, ZRotation);
		m_pCmanager->SetCameraPosition(Qrotation.RotationV3(*m_pCmanager->GetCameraPosition()));
	}
	if ((nChar == 0x28) && m_pCmanager->OrbitBelowCheck())//down arrow
	{
		Quaternion Qrotation = Quaternion(-XRotation, 0, -ZRotation);
		m_pCmanager->SetCameraPosition(Qrotation.RotationV3(*m_pCmanager->GetCameraPosition()));
	}
	if (nChar == 0x25)//left arrow
	{
		Quaternion Qrotation = Quaternion(0.0f, 0.05f, 0.0f);
		m_pCmanager->SetCameraPosition(Qrotation.RotationV3(*m_pCmanager->GetCameraPosition()));
	}
	if (nChar == 0x27)//right arrow
	{
		Quaternion Qrotation = Quaternion(0.0f, -0.05f, 0.0f);
		m_pCmanager->SetCameraPosition(Qrotation.RotationV3(*m_pCmanager->GetCameraPosition()));
	}
	m_bAltDown = bAltDown;
	m_pFmanager->update(nChar);
}

void InputManager::CatchMouse ( bool bLeftButtonDown, bool bRightButtonDown, bool bMiddleButtonDown,
							    bool bSideButton1Down, bool bSideButton2Down, int nMouseWheelDelta,
							    int xPos, int yPos )
{
	
	m_bLeftButtonDown = bLeftButtonDown; 
	if(m_bLeftButtonDown){m_pFmanager->SetcMouseButtonL(Green);} else {m_pFmanager->SetcMouseButtonL(Red);}
	m_bRightButtonDown = bRightButtonDown;
	if(bRightButtonDown){m_pFmanager->SetcMouseButtonR(Green);} else {m_pFmanager->SetcMouseButtonR(Red);}
	m_bMiddleButtonDown = bMiddleButtonDown;
	m_bSideButton1Down = bSideButton1Down;
	m_bSideButton2Down = bSideButton2Down;
	m_iMouseWheelDelta = nMouseWheelDelta;
	m_ixPos = xPos; 
	m_pFmanager->SetMouseXwchar(m_ixPos);
	m_iyPos = yPos;
	m_pFmanager->SetMouseYwchar(m_iyPos);

	//Considered using update call to effect input events but choose direct methods for now
	
	//Method calls here
	MouseWheel();
	MouseButtons ();
	MouseOver();


}

void InputManager::MouseWheel ()
{
	//ZoomInput
	D3DXVECTOR3 eyeNormal;
	if (m_iMouseWheelDelta < 0)
	{
		m_pCmanager->ZoomIn();
	}
	if (m_iMouseWheelDelta > 0)
	{
		m_pCmanager->ZoomOut();
	}
}

void InputManager::MouseButtons ()
{
	
	D3DXVECTOR3 viewvector = m_pCmanager->GetViewVector(); //use the x->z and z_x relationship
	viewvector.y = 0;
	D3DXVec3Normalize(&viewvector, &viewvector);
	float XRotation = (viewvector.z / 100);
	float ZRotation = -(viewvector.x / 100); // Some day I'll realise why this needs to be negative, perhaps due to left handed system
	float YRotation = -0.005;

	//RotationInput
	if (m_bLeftButtonDown && m_bRightButtonDown)//Start rotating by delta
	{

		float mouseDeltaX = m_ixPos - m_ixPosPrevious;
		float mouseDeltaY = m_iyPos - m_iyPosPrevious;
		XRotation *= mouseDeltaY;
		ZRotation *= mouseDeltaY;
		YRotation *= -mouseDeltaX;
		Quaternion Qrotation = Quaternion(XRotation, YRotation, ZRotation);
		m_pCmanager->SetCameraPosition(Qrotation.RotationV3(*m_pCmanager->GetCameraPosition()));
	}

	//Selects/de-selects or moves a piece
	if (m_bLeftButtonDown && !m_bRightButtonDown)
	{
	
		std::list<Piece*>::iterator i;

		for(i=m_lPieces->begin(); i != m_lPieces->end(); ++i)
		{		
			Piece* x = *i;
			CollisionResult collisionResult = m_pCollmanager->MouseOverModelCheck(m_ixPos, m_iyPos, 
				x->GetMesh(), x->GetWorldMatrix());
			if (collisionResult.result == TRUE && 
				((UINT)x->GetPieceColour() == m_pServices->GetService<BoardManager>("BoardManager")->GetPieceColour()))
			{
				if (m_pSelected == NULL)
				{
					m_pSelected = x;
					m_pSelected->Effect_Select_Selected();
				}
				else if (m_pSelected == x)
				{
					m_pSelected->Effect_Select_Reset();
					m_pSelected = NULL;
				}
			}

		}
		

	}

	m_ixPosPrevious = m_ixPos;
	m_iyPosPrevious = m_iyPos;
}

void InputManager::MouseOver()
{
	//Piece selection
	std::list<Piece*>::iterator i;

	for(i=m_lPieces->begin(); i != m_lPieces->end(); ++i)
	{		
		Piece* x = *i;
		CollisionResult collisionResult = m_pCollmanager->MouseOverModelCheck(m_ixPos, m_iyPos, 
			x->GetMesh(), x->GetWorldMatrix());
		if (collisionResult.result == TRUE && 
			((UINT)x->GetPieceColour() == m_pServices->GetService<BoardManager>("BoardManager")->GetPieceColour()) &&
			m_pSelected == NULL)
		{
			if (m_pHighlighted == NULL)
			{
				m_pHighlighted = x;
				m_pHighlighted->Effect_Select_Highlight();
				return;
			}
			else
			{
				m_pHighlighted->Effect_Select_Reset();
				m_pHighlighted = x;
				m_pHighlighted->Effect_Select_Highlight();
				return;
			}
		}
		else if(m_pHighlighted != NULL && m_pSelected == NULL)
		{
			m_pHighlighted->Effect_Select_Reset();
			m_pHighlighted = NULL;
		}

	}


	//Piece Placement
	//For this we assume the chessboard is the first item in the model 
	//list as it is currently always added first.
	if  (m_pSelected != NULL)
	{

		D3DXVECTOR3 foundPos;
		int j;
		bool detected = false;

		D3DXVECTOR3 rayOrg;
		D3DXVECTOR3 rayDelta;

		PickRay(m_ixPos, m_iyPos, rayOrg, rayDelta);

		m_pFmanager->m_v3PickOrg = rayOrg;
		m_pFmanager->m_v3PickDelta = rayDelta;

		RayAABB pickingray;
		make_ray(rayOrg,rayDelta,&pickingray);

		for (j = 119; j > 0; (j -= 16))
		{
			int k;

			for (k = 0; k < 7; k++)
			{
								
				m_pFmanager->m_fPosition = j-k;

				AABB testBox = m_pBoardmanager->GetPositionAABB(j-k);
				bool testresult = slope(&pickingray, &testBox);

				m_pFmanager->m_fVart = testresult;
				m_pFmanager->m_v3Min = testBox.minPt;
				m_pFmanager->m_v3Max = testBox.maxPt;

				if(testresult)
				{
					foundPos = D3DXVECTOR3(m_pBoardmanager->GetPosition3D(j-k));
					detected =  true;
				}
				if(detected)break;
			}
			if(detected)break;
		}


		if(detected && m_pMove == NULL)
		{
			g_lMoveModels.clear();
			m_pMove = m_pSelected;
			m_pMove->m_v3Moveto = foundPos;
			g_lMoveModels.push_back(m_pMove);
		}
		else if (!detected && m_pMove != NULL)
		{
			g_lMoveModels.clear();
			m_pMove = NULL;
		}
	}
}


void InputManager::PickRay(int xPos, int yPos, D3DXVECTOR3 &rayOrg, D3DXVECTOR3 &rayDelta)
{
	D3DXMATRIX P = *m_pCmanager->GetProjectionMatrix();

	//get the picking direction out of 2d into viewspace
	rayDelta.x =  ( ( ( 2.0f * xPos ) / 800  ) - 1 ) / P(0,0);
	rayDelta.y =  ( ( ( -2.0f * yPos ) / 600 ) + 1 ) / P(1,1);
	rayDelta.z = 1;

	D3DXVECTOR3 rayOriginView(0.0f, 0.0f, 0.0f);//origin starts at 0 for view

	//convert to world via invViewMatrix

	D3DXMATRIX m;

	D3DXMatrixInverse( &m, NULL, m_pCmanager->GetViewMatrix());

	D3DXVec3TransformCoord( &rayOrg, &rayOriginView, &m);
	D3DXVec3TransformNormal(&rayDelta, &rayDelta, &m);

}