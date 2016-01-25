#include "DXUT.h" 
#include "InputManager.h"

InputManager::InputManager()
{
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

void InputManager::Initialise(FontManager* Fmanager, CameraManager* CManager, 
							  CollisionManager* CollManager, std::list<CustomModel*>& models,
							  D3DXVECTOR4& forcePosition)
{
	m_pFmanager = Fmanager;
	m_pCmanager = CManager;
	m_pCollmanager = CollManager;
	m_lModels = &models;
	m_pForcePosition = &forcePosition;
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
	//Colour Fonts
	m_bLeftButtonDown = bLeftButtonDown; 
	if(m_bLeftButtonDown){m_pFmanager->SetcMouseButtonL(Green);} else {m_pFmanager->SetcMouseButtonL(Red);}
	m_bRightButtonDown = bRightButtonDown;
	if(bRightButtonDown){m_pFmanager->SetcMouseButtonR(Green);} else {m_pFmanager->SetcMouseButtonR(Red);}
	//Save mouse history
	m_bMiddleButtonDown = bMiddleButtonDown;
	m_bSideButton1Down = bSideButton1Down;
	m_bSideButton2Down = bSideButton2Down;
	m_iMouseWheelDelta = nMouseWheelDelta;
	//save coords and update font
	m_ixPos = xPos; 
	m_pFmanager->SetMouseXwchar(xPos);
	m_iyPos = yPos;
	m_pFmanager->SetMouseYwchar(yPos);

	//Considered using update call to effect input events but choose direct methods for now
	
	//Method calls here
	MouseWheel();
	MouseButtons ();


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

	//RotationInput
	if (m_bLeftButtonDown && m_bRightButtonDown)//Start rotating 
	{
		D3DXVECTOR3 viewvector = m_pCmanager->GetViewVector(); //use the x->z and z_x relationship
		viewvector.y = 0;
		D3DXVec3Normalize(&viewvector, &viewvector);
		float XRotation = (viewvector.z / 100);
		float ZRotation = -(viewvector.x / 100); // Some day I'll realise why this needs to be negative, perhaps due to left handed system
		float YRotation = -0.005;
		
		float mouseDeltaX = m_ixPos - m_ixPosPrevious;
		float mouseDeltaY = m_iyPos - m_iyPosPrevious;
		XRotation *= -mouseDeltaY;
		ZRotation *= -mouseDeltaY;
		YRotation *= mouseDeltaX;
		Quaternion Qrotation = Quaternion(XRotation, YRotation, ZRotation);
		m_pCmanager->SetCameraPosition(Qrotation.RotationV3(*m_pCmanager->GetCameraPosition()));
	}

	if (m_bLeftButtonDown && !m_bRightButtonDown)//move deformation point
	{
		std::list<CustomModel*>::iterator i;

		for(i=m_lModels->begin(); i != m_lModels->end(); ++i)
		{		
			CustomModel* x = *i;
			CollisionResult collisionResult = m_pCollmanager->pointerCollision(m_ixPos, m_iyPos, 
																		x->GetMesh(), x->GetWorldMatrix());
			if (collisionResult.result == TRUE)
			{
				D3DXVECTOR3 vpos = x->GetFaceVertex(collisionResult.faceCollision);
				D3DXVec3TransformCoord(&vpos, &vpos, x->GetWorldMatrix());
				D3DXVECTOR4 vpos4 = vpos;
				D3DXVECTOR4 offset = (vpos4 - x->GetPosition());
				vpos4 += offset;
				*m_pForcePosition = vpos4;

			}

		};
		
		
	}


	//remember previous mouse coords
	m_ixPosPrevious = m_ixPos;
	m_iyPosPrevious = m_iyPos;
}