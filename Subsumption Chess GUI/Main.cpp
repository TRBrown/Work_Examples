//--------------------------------------------------------------------------------------
// File: EmptyProject.cpp
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//--------------------------------------------------------------------------------------
#include "DXUT.h"

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include "Custom_Services.h"
#include "resource.h"
#include <windows.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#pragma warning(disable:4995)
#include <list>
#include "Models.h"
#include "Manager.h"
#include "CameraManager.h"
#include "BoardManager.h"
#include "CameraManager.h"
#include "FontManager.h"
#include "ThreadManager.h"
#include "InputManager.h"
#include "CollisionManager.h"
#include "ModelSource\AABBRender.h"

IDirect3DDevice9* g_pD3DDevice;
std::list<CustomModel*> g_lModels;
std::list<Piece*> g_lMoveModels;
std::list<Piece*> g_lPieces;

D3DLIGHT9 light1;
D3DLIGHT9 light2;

static Custom_Services* GameManagers;
static CameraManager* CManager;
static BoardManager* BManager;
static FontManager* FManager;
static ThreadManager* AIManager;
static InputManager* IManager;
static CollisionManager* CollManager;

UINT g_uiGamestate = 0; // set initial state to Player Move 0 = player, 1 = AI, 2 = AI moving
UINT g_uiCurrentMove = 0; // Unused currently. Current move white or black
UINT g_uiMoveTimer = 9999999;


void CreatePointLight(D3DVECTOR &pos, D3DLIGHT9 &light)
{

		
		ZeroMemory( &light, sizeof(D3DLIGHT9) );
 		light.Ambient.r  = 0.1f;
        light.Ambient.g  = 0.1f;
        light.Ambient.b  = 0.1f;
        light.Ambient.a  = 1.0f;       
		light.Diffuse.r  = 0.4f;
        light.Diffuse.g  = 0.4f;
        light.Diffuse.b  = 0.4f;
        light.Diffuse.a  = 1.0f;
		light.Specular.r  = 0.5f;
        light.Specular.g  = 0.5f;
        light.Specular.b  = 0.5f;
        light.Specular.a  = 1.0f;
         
        light.Position = pos;
         
        light.Attenuation0= 1.0f; 

}
void CreateDirectionLight(D3DVECTOR &dir, D3DLIGHT9 &light)
{


	ZeroMemory( &light, sizeof(D3DLIGHT9) );
	light.Ambient.r  = 0.1f;
	light.Ambient.g  = 0.1f;
	light.Ambient.b  = 0.1f;
	light.Ambient.a  = 1.0f;       
	light.Diffuse.r  = 0.5f;
	light.Diffuse.g  = 0.5f;
	light.Diffuse.b  = 0.5f;
	light.Diffuse.a  = 1.0f;
	light.Specular.r  = 0.5f;
	light.Specular.g  = 0.5f;
	light.Specular.b  = 0.5f;
	light.Specular.a  = 1.0f;

	light.Direction = dir;

	light.Attenuation0= 1.0f; 

}


//--------------------------------------------------------------------------------------
// Rejects any D3D9 devices that aren't acceptable to the app by returning false
//--------------------------------------------------------------------------------------
bool CALLBACK IsD3D9DeviceAcceptable( D3DCAPS9* pCaps, D3DFORMAT AdapterFormat, D3DFORMAT BackBufferFormat,
                                      bool bWindowed, void* pUserContext )
{
    // Typically want to skip back buffer formats that don't support alpha blending
    IDirect3D9* pD3D = DXUTGetD3D9Object();
    if( FAILED( pD3D->CheckDeviceFormat( pCaps->AdapterOrdinal, pCaps->DeviceType,
                                         AdapterFormat, D3DUSAGE_QUERY_POSTPIXELSHADER_BLENDING,
                                         D3DRTYPE_TEXTURE, BackBufferFormat ) ) )
        return false;

    return true;
}


//--------------------------------------------------------------------------------------
// Before a device is created, modify the device settings as needed
//--------------------------------------------------------------------------------------
bool CALLBACK ModifyDeviceSettings( DXUTDeviceSettings* pDeviceSettings, void* pUserContext )
{
	return true;
}


//--------------------------------------------------------------------------------------
// Create any D3D9 resources that will live through a device reset (D3DPOOL_MANAGED)
// and aren't tied to the back buffer size
//--------------------------------------------------------------------------------------
HRESULT CALLBACK OnD3D9CreateDevice( IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc,
                                     void* pUserContext )
{
	
	g_pD3DDevice = pd3dDevice;
	//HRESULT hr;

	//pd3dDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// Turn on the z buffer
    pd3dDevice->SetRenderState( D3DRS_ZENABLE, TRUE );

    // Turn on ambient lighting 
    pd3dDevice->SetRenderState( D3DRS_AMBIENT, 0xffffffff );
	
	// Turn on Wire frame
	//pd3dDevice->SetRenderState( D3DRS_FILLMODE, D3DFILL_WIREFRAME );

	D3DVECTOR val1; val1.x = 0.0f; val1.y = 10.0f; val1.z = 1.0f;
	CreatePointLight(val1,light1);
	D3DVECTOR val2; val2.x = 0.0f; val2.y = 0.5f; val2.z = 1.0f;
	CreateDirectionLight(val2,light2);

	//**Initialise these sometime instead of parsing via instance**
	GameManagers = Custom_Services::Instance();

	CManager = CameraManager::Instance();
	GameManagers->AddService<CameraManager>(CManager, "CameraManager");
	CollManager = CollisionManager::Instance();
	CollManager->Initialise(CManager);
	GameManagers->AddService<CollisionManager>(CollManager, "CollisionManager");
	BManager = BoardManager::Instance(g_lModels, g_lPieces, pd3dDevice, GameManagers);
	GameManagers->AddService<BoardManager>(BManager, "BoardManager");
	FManager = new FontManager(pd3dDevice, &g_uiGamestate, GameManagers);
	GameManagers->AddService<FontManager>(FManager, "FontManager");
	AIManager = ThreadManager::Instance(GameManagers);
	GameManagers->AddService<ThreadManager>(AIManager, "ThreadManager");
	IManager = InputManager::Instance();
	GameManagers->AddService<InputManager>(IManager, "InputManager");
	IManager->Initialise(GameManagers, g_lPieces, g_lModels);

	return S_OK;
}


//--------------------------------------------------------------------------------------
// Create any D3D9 resources that won't live through a device reset (D3DPOOL_DEFAULT) 
// or that are tied to the back buffer size 
//--------------------------------------------------------------------------------------
HRESULT CALLBACK OnD3D9ResetDevice( IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc,
                                    void* pUserContext )
{	
	return S_OK;
}


//--------------------------------------------------------------------------------------
// Handle updates to the scene.  This is called regardless of which D3D API is used
//--------------------------------------------------------------------------------------
void CALLBACK OnFrameMove( double fTime, float fElapsedTime, void* pUserContext )
{



	std::list<CustomModel*>::iterator i;

	if (g_uiGamestate == 1)
	{
		BManager->InitGenerateMoves();
		AIManager->CreateAIThreadObject(BManager->m_iAIMovenumber);
		g_uiGamestate = 2;
	}

	for(i=g_lModels.begin(); i != g_lModels.end(); ++i)
	{		
		CustomModel* x = *i;
		x->update();

	}

	if (UINT x =BManager->GetMoveTimer() == 0)
	{ 
		D3DXVECTOR3 tmp = BManager->GetAIMove();
		BManager->MakeAIMove(tmp);
		BManager->SetMoveTimer(g_uiMoveTimer);
		FManager->Stateupdate();
	}

}


//--------------------------------------------------------------------------------------
// Render the scene using the D3D9 device
//--------------------------------------------------------------------------------------
void CALLBACK OnD3D9FrameRender( IDirect3DDevice9* pd3dDevice, double fTime, float fElapsedTime, void* pUserContext )
{
    HRESULT hr;
	std::list<CustomModel*>::iterator i;
	std::list<Piece*>::iterator y;
	std::list<AABBRender*>::iterator z;

	// Clear the render target and the zbuffer 
    V( pd3dDevice->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_ARGB( 0, 0, 0, 0 ), 1.0f, 0 ) );
	
	if( SUCCEEDED( pd3dDevice->BeginScene() ) )
	{
		for(i=g_lModels.begin(); i != g_lModels.end(); ++i)
		{		
			CustomModel* x = *i;
			if(x->GetVisible()) // If visible
			{
				x->AssignParameters(light1, light2, CManager->GetCameraPosition(), CManager->GetViewMatrix(), 
					CManager->GetProjectionMatrix());
				x->render();
			}

		};
		
		for(y=g_lMoveModels.begin(); y != g_lMoveModels.end(); ++y)
		{		
			Piece* x = *y;
			if(x->GetVisible()) // If visible
			{
				x->render(x->m_v3Moveto);
			}

		};

//		Rendering of pick collision objects used to move pieces with the cursor
// 		for(z = BManager->p_lPositionAABBs.begin(); z != BManager->p_lPositionAABBs.end(); ++z)
// 		{		
// 			AABBRender* x = *z;
// 
// 			x->render(*CManager->GetViewMatrix(), *CManager->GetProjectionMatrix());
// 
// 		};

		FManager->render();

		V( pd3dDevice->EndScene());
	}
}


//--------------------------------------------------------------------------------------
// Handle messages to the application 
//--------------------------------------------------------------------------------------
LRESULT CALLBACK MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam,
                          bool* pbNoFurtherProcessing, void* pUserContext )
{
    return 0;
}


//--------------------------------------------------------------------------------------
// Release D3D9 resources created in the OnD3D9ResetDevice callback 
//--------------------------------------------------------------------------------------
void CALLBACK OnD3D9LostDevice( void* pUserContext )
{
}


//--------------------------------------------------------------------------------------
// Release D3D9 resources created in the OnD3D9CreateDevice callback 
//--------------------------------------------------------------------------------------
void CALLBACK OnD3D9DestroyDevice( void* pUserContext )
{

	std::list<CustomModel*>::iterator i;

	for(i=g_lModels.begin(); i != g_lModels.end(); ++i)
	{		
		delete *i;
	};

	//None static "atexit()" registered class's
	delete FManager;

}

//--------------------------------------------------------------------------------------
// Handle key presses
//--------------------------------------------------------------------------------------
void CALLBACK OnKeyboard( UINT nChar, bool bKeyDown, bool bAltDown, void* pUserContext )
{
	IManager->CatchKeyboardDown(nChar, bKeyDown, bAltDown);	
}

//--------------------------------------------------------------------------------------
// Handle mouse button presses
//--------------------------------------------------------------------------------------
void CALLBACK OnMouse( bool bLeftButtonDown, bool bRightButtonDown, bool bMiddleButtonDown,
					  bool bSideButton1Down, bool bSideButton2Down, int nMouseWheelDelta,
					  int xPos, int yPos, void* pUserContext )
{

	IManager->CatchMouse(bLeftButtonDown, bRightButtonDown, bMiddleButtonDown,
						  bSideButton1Down, bSideButton2Down, nMouseWheelDelta,
						  xPos, yPos);

}

//--------------------------------------------------------------------------------------
// Initialize everything and go into a render loop
//--------------------------------------------------------------------------------------


INT WINAPI wWinMain( HINSTANCE, HINSTANCE, LPWSTR, int )
{
    // Enable run-time memory check for debug builds.
#if defined(DEBUG) | defined(_DEBUG)
    _CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
#endif

    // Set the callback functions
    DXUTSetCallbackD3D9DeviceAcceptable( IsD3D9DeviceAcceptable );
    DXUTSetCallbackD3D9DeviceCreated( OnD3D9CreateDevice );
    DXUTSetCallbackD3D9DeviceReset( OnD3D9ResetDevice );
    DXUTSetCallbackD3D9FrameRender( OnD3D9FrameRender );
    DXUTSetCallbackD3D9DeviceLost( OnD3D9LostDevice );
    DXUTSetCallbackD3D9DeviceDestroyed( OnD3D9DestroyDevice );
    DXUTSetCallbackDeviceChanging( ModifyDeviceSettings );
    DXUTSetCallbackKeyboard( OnKeyboard );
	DXUTSetCallbackMouse( OnMouse, true );
	DXUTSetCallbackMsgProc( MsgProc );
    DXUTSetCallbackFrameMove( OnFrameMove );

    // TODO: Perform any application-level initialization here

    // Initialize DXUT and create the desired Win32 window and Direct3D device for the application
    DXUTInit( true, true ); // Parse the command line and show msgboxes
    DXUTSetHotkeyHandling( true, true, true );  // handle the default hotkeys
    DXUTSetCursorSettings( true, true ); // Show the cursor and clip it when in full screen
    DXUTCreateWindow( L"CG Assignment 1" );
    DXUTCreateDevice( true, 800, 600 );

    // Start the render loop
    DXUTMainLoop();

    // TODO: Perform any application-level cleanup here

    return DXUTGetExitCode();
}
