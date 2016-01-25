#ifndef FONTMANAGER
#define FONTMANAGER
#pragma warning(disable:4995)

#include "d3dx9.h" 
#include "d3d9.h"
#include "BoardManager.h"
#include <TCHAR.H>
#include <sstream>
#include <string>
#include <vector>
#include <malloc.h>
#include "Custom_Services.h"


#define Red		(D3DCOLOR_ARGB(255,205,38,38))
#define Green	(D3DCOLOR_ARGB(255,47,79,47))
#define White	(D3DCOLOR_ARGB(255,200,200,200))
#define DarkRed	(D3DCOLOR_ARGB(255,192,0,0))
#define SkyBlue	(D3DCOLOR_ARGB(255,135,206,235))

class FontManager
{
private:

	IDirect3DDevice9*		m_pD3dDevice;	
	ID3DXFont*				m_pMoveTextFont;
	ID3DXFont*				m_pMouseControlFont;
	ID3DXFont*				m_pDataFont;
	UINT*					m_pGamestate;
	BoardManager*			m_pBManager;
	Custom_Services*		m_pGameManagers;

	//keyboard movement input
	RECT					m_rMRct;
	RECT					m_rSRct;
	RECT					m_rTRct;
	RECT					m_rDRct;
	//mouse input controls
	RECT					m_rLMB;
	RECT					m_rRMB;
	RECT					m_rMouseX;
	RECT					m_rMouseY;
	RECT					m_rRotationxyz;
	RECT					m_rQuaternion;

	D3DCOLOR				m_cMoveTextColor;
	D3DCOLOR				m_cMouseXY;
	D3DCOLOR				m_cMouseButtonL;
	D3DCOLOR				m_cMouseButtonR;
	D3DCOLOR				m_cRotationxyz;
	D3DCOLOR				m_cQuaternion;


	UINT					m_uiUIstate;
	std::vector<float>		m_vMove;


	std::string				m_sInput;
	wchar_t*				m_wcInput;
	wchar_t*				m_wcMessage;
	wchar_t*				m_wcMoveSrc;
	wchar_t*				m_wcMoveDest;
	wchar_t*				m_wcMouseX;
	wchar_t*				m_wcMouseY;

	wchar_t					m_wcBlank;



protected:


public:

	D3DXVECTOR3				m_v3CamOrg;
	D3DXVECTOR3				m_v3PickOrg;
	D3DXVECTOR3				m_v3PickDelta;
	float					m_fVart;
	float					m_fPosition;
	D3DXVECTOR3				m_v3Min;
	D3DXVECTOR3				m_v3Max;
	bool					m_bError;
	bool					m_bVisible;
	HRESULT					hr;

	FontManager(IDirect3DDevice9* pd3dDevice, UINT* Gamestate, 
				Custom_Services* GameManagers);

	FontManager(const FontManager &);

	virtual ~FontManager();

	void Preload();
	void update(UINT nChar);
	void render();
	void Updatekeystroke(char keystroke);
	void Stateupdate();
	void Moveupdate(int keystroke);

	bool Error()const{return m_bError;} 

	//Accessors
	void SetcMouseButtonL(const D3DCOLOR colour){m_cMouseButtonL = colour;}
	void SetcMouseButtonR(const D3DCOLOR colour){m_cMouseButtonR = colour;}
	void SetMouseXwchar(const int x)
	{
		std::string tmp = "MouseX ";
		std::ostringstream oss;
		oss << x;
		tmp += oss.str();

		DWORD size = MultiByteToWideChar(CP_ACP, 0, tmp.c_str(), -1, NULL, 0);
		if (m_wcMouseX != NULL) delete [] m_wcMouseX;
		m_wcMouseX = new wchar_t[size];
		MultiByteToWideChar(CP_ACP, 0, tmp.c_str(), -1, m_wcMouseX, size);

	}
	void SetMouseYwchar(const int y)
	{
		std::string tmp = "MouseY ";
		std::ostringstream oss;
		oss << y;
		tmp += oss.str();

		int size = MultiByteToWideChar(CP_ACP, 0, tmp.c_str(), -1, 0, 0);
		if (m_wcMouseY != NULL) delete [] m_wcMouseY;
		m_wcMouseY = (wchar_t*)malloc(size * sizeof(wchar_t));
		MultiByteToWideChar(CP_ACP, 0, tmp.c_str(), -1, m_wcMouseY, size);

	}

};




#endif

