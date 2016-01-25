#include "DXUT.h" 
#include "FontManager.h"

//wchar_t					m_wcPlayerMessage[] = L"Your Move";
//wchar_t					m_wcToMessage[] = L"To";
//wchar_t					m_wcAIMessage[] = L"The AI is Moving";

wchar_t					m_wcLeft[] = L"Left";
wchar_t					m_wcRight[] = L"Right";



FontManager::FontManager(IDirect3DDevice9* pd3dDevice)
{
	m_pD3dDevice = pd3dDevice;

//	D3DXCreateFont( m_pD3dDevice, 20, 0, FW_BOLD, 0, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, 
//		DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, TEXT("Arial"), &m_pMoveTextFont );

	D3DXCreateFont( m_pD3dDevice, 10, 0, FW_LIGHT, 0, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, 
		DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, TEXT("Lucida Sans"), &m_pMouseControlFont );
	
	m_wcInput = NULL;
	m_wcMouseX = NULL;
	m_wcMouseY = NULL;

//	m_pGamestate = Gamestate;
//	m_uiUIstate = 0;
//	m_pBManager = BManager;
	m_vMove.clear();
	Preload();
}


FontManager::~FontManager()

{
//	m_pMoveTextFont->Release();
//	m_pMoveTextFont = 0;
	m_pMouseControlFont->Release();
	m_pMouseControlFont = 0;

	delete [] m_wcMouseX;
	delete [] m_wcMouseY;


}


void FontManager::Preload()
{
	
	// Create a colour for thqe text - in this case blue
	m_cMoveTextColor = D3DCOLOR_ARGB(255,200,200,200);   
	m_cMouseButtonL = Red;
	m_cMouseButtonR = Red;
	m_cMouseXY = SkyBlue;

	//Process wchar's


//	m_wcMessage = m_wcPlayerMessage;

	m_wcBlank = NULL;
	m_wcMoveDest = &m_wcBlank;
	m_wcMoveSrc = &m_wcBlank;
	m_wcInput = &m_wcBlank;


	// Create a rectangle to indicate where on the screen it should be drawn
	m_rMRct.left=2;
	m_rMRct.right=130;
	m_rMRct.top=10;
	m_rMRct.bottom=m_rMRct.top+20;

	m_rSRct.left=131;
	m_rSRct.right=160;
	m_rSRct.top=10;
	m_rSRct.bottom=m_rSRct.top+20;

	m_rTRct.left=161;
	m_rTRct.right=190;
	m_rTRct.top=10;
	m_rTRct.bottom=m_rTRct.top+20;

	m_rDRct.left=191;
	m_rDRct.right=220;
	m_rDRct.top=10;
	m_rDRct.bottom=m_rDRct.top+20;

	m_rLMB.left=710;
	m_rLMB.right=750;
	m_rLMB.top=10;
	m_rLMB.bottom=m_rDRct.top+10;

	m_rRMB.left=760;
	m_rRMB.right=800;
	m_rRMB.top=10;
	m_rRMB.bottom=m_rDRct.top+10;

	m_rMouseX.left=680;
	m_rMouseX.right=730;
	m_rMouseX.top=30;
	m_rMouseX.bottom=m_rMouseX.top+10;

	m_rMouseY.left=740;
	m_rMouseY.right=800;
	m_rMouseY.top=30;
	m_rMouseY.bottom=m_rMouseY.top+10;

	m_rRotationxyz.left=760;
	m_rRotationxyz.right=800;
	m_rRotationxyz.top=50;
	m_rRotationxyz.bottom=m_rDRct.top+10;

	m_rQuaternion.left=710;
	m_rQuaternion.right=750;
	m_rQuaternion.top=50;
	m_rQuaternion.bottom=m_rDRct.top+10;

};


void FontManager::update(UINT nChar)
{

	switch ( nChar )
	{
	case 8:
		Updatekeystroke('X');
		Moveupdate('X');
		break;
	case 13:
		Stateupdate();
		break;
	case 49:
		Updatekeystroke('1');
		Moveupdate(0);
		break;
	case 50:
		Updatekeystroke('2');
		Moveupdate(1);
		break;
	case 51:
		Updatekeystroke('3');
		Moveupdate(2);
		break;
	case 52:
		Updatekeystroke('4');
		Moveupdate(3);
		break;
	case 53:
		Updatekeystroke('5');
		Moveupdate(4);
		break;
	case 54:
		Updatekeystroke('6');
		Moveupdate(5);
		break;
	case 55:
		Updatekeystroke('7');
		Moveupdate(6);
		break;
	case 56:
		Updatekeystroke('8');
		Moveupdate(7);
		break;
	case 65:
		Updatekeystroke('A');
		Moveupdate(0);
		break;
	case 66:
		Updatekeystroke('B');
		Moveupdate(1);
		break;
	case 67:
		Updatekeystroke('C');
		Moveupdate(2);
		break;
	case 68:
		Updatekeystroke('D');
		Moveupdate(3);
		break;
	case 69:
		Updatekeystroke('E');
		Moveupdate(4);
		break;
	case 70:
		Updatekeystroke('F');
		Moveupdate(5);
		break;
	case 71:
		Updatekeystroke('G');
		Moveupdate(6);
		break;
	case 72:
		Updatekeystroke('H');
		Moveupdate(7);
		break;
	}

}

// Draw text
void FontManager::render()
{

	if(m_uiUIstate == 0)m_wcMoveSrc = m_wcInput;
	else m_wcMoveDest = m_wcInput;
	
//	m_pMoveTextFont->DrawText(NULL, m_wcMessage, -1, &m_rMRct, 0, m_cMoveTextColor );
//	m_pMoveTextFont->DrawText(NULL, m_wcMoveSrc, -1, &m_rSRct, 0, m_cMoveTextColor );
//	m_pMoveTextFont->DrawText(NULL, m_wcToMessage, -1, &m_rTRct, 0, m_cMoveTextColor );
//	m_pMoveTextFont->DrawText(NULL, m_wcMoveDest, -1, &m_rDRct, 0, m_cMoveTextColor );

	m_pMouseControlFont->DrawText(NULL, m_wcLeft, -1, &m_rLMB, 0, m_cMouseButtonL );
	m_pMouseControlFont->DrawText(NULL, m_wcRight, -1, &m_rRMB, 0, m_cMouseButtonR );
	m_pMouseControlFont->DrawText(NULL, m_wcMouseX, -1, &m_rMouseX, 0, m_cMouseXY );
	m_pMouseControlFont->DrawText(NULL, m_wcMouseY, -1, &m_rMouseY, 0, m_cMouseXY );

};


void FontManager::Updatekeystroke(char keystroke)
{
	

};

//for game state conditional events when enter is pressed
void FontManager::Stateupdate()
{

	switch ( '0' )
	{
		case 0:

			break;
		case 1:

			break;
		case 2:

			break;
	}

};

void FontManager::Moveupdate(int keystroke)
{
	

};