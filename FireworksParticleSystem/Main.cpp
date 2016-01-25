//--------------------------------------------------------------------------------------
// Author - Thomas Brown
//
// Notes - 
//
// Controls - left mouse button to change deformation position
//			- both mouse buttons to move/rotate the camera
//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------
// DXUT Copyright (c) Microsoft Corporation. All rights reserved.
//--------------------------------------------------------------------------------------
#include "DXUT.h"

#define TIXML_USE_STL
#define _CRT_SECURE_NO_WARNINGS

#include "resource.h"
#include <time.h>
#include "stdlib.h"
#include <string>
#pragma warning(disable:4995)
#include <list>
#include "CameraManager.h"
#include "FontManager.h"
#include "InputManager.h"
#include "CollisionManager.h"
#include "Director.h"


FontManager* FManager;
static InputManager* IManager;
static CollisionManager* CollManager;
static Director* pDirector;

D3DLIGHT9 light1;
D3DLIGHT9 light2;


void CreatePointLight(D3DVECTOR &pos, D3DLIGHT9 &light)
{

		
		ZeroMemory( &light, sizeof(D3DLIGHT9) );
 		light.Ambient.r  = 0.1f;
        light.Ambient.g  = 0.1f;
        light.Ambient.b  = 0.1f;
        light.Ambient.a  = 1.0f;       
		light.Diffuse.r  = 1.0f;
        light.Diffuse.g  = 1.0f;
        light.Diffuse.b  = 1.0f;
        light.Diffuse.a  = 1.0f;
		light.Specular.r  = 0.4f;
        light.Specular.g  = 0.4f;
        light.Specular.b  = 0.4f;
        light.Specular.a  = 1.0f;
         
        light.Position = pos;
         
        light.Attenuation0= 1.0f; 

}
void CreateDirectionLight(D3DVECTOR &dir, D3DLIGHT9 &light)
{


	ZeroMemory( &light, sizeof(D3DLIGHT9) );
	light.Ambient.r  = 0.0f;
	light.Ambient.g  = 0.0f;
	light.Ambient.b  = 0.0f;
	light.Ambient.a  = 1.0f;       
	light.Diffuse.r  = 0.0f;
	light.Diffuse.g  = 0.0f;
	light.Diffuse.b  = 0.0f;
	light.Diffuse.a  = 1.0f;
	light.Specular.r  = 0.0f;
	light.Specular.g  = 0.0f;
	light.Specular.b  = 0.0f;
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
	
	//HRESULT hr;

	// Turn on the z buffer
    pd3dDevice->SetRenderState( D3DRS_ZENABLE, TRUE );

    // Turn on ambient lighting 
    //pd3dDevice->SetRenderState( D3DRS_AMBIENT, 0xffffffff );
	
	// Turn on Wire frame
	//pd3dDevice->SetRenderState( D3DRS_FILLMODE, D3DFILL_WIREFRAME );

	D3DVECTOR val1; val1.x = 0.0f; val1.y = 0.0f; val1.z = -150.0f;
	CreatePointLight(val1,light1);
	D3DVECTOR val2; val2.x = 0.0f; val2.y = 0.0f; val2.z = 1.0f;
	CreateDirectionLight(val2,light2);


	gCManager = CameraManager::Instance();
	FManager = new FontManager(pd3dDevice);
	CollManager = CollisionManager::Instance();
	CollManager->Initialise(gCManager);
	IManager = InputManager::Instance();
	IManager->Initialise(FManager, gCManager, CollManager);
	pDirector = Director::Instance();
	pDirector->Initialise(pd3dDevice);
	pDirector->load("fireworks.xml");
	InitAllVertexDeclarations(pd3dDevice);


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

	pDirector->UpdateScene(fElapsedTime);

}


//--------------------------------------------------------------------------------------
// Render the scene using the D3D9 device
//--------------------------------------------------------------------------------------
void CALLBACK OnD3D9FrameRender( IDirect3DDevice9* pd3dDevice, double fTime, float fElapsedTime, void* pUserContext )
{
	HRESULT hr;
	// Clear the render target and the zbuffer 
	V( pd3dDevice->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_ARGB( 0, 0, 0, 0 ), 1.0f, 0 ) );

	if( SUCCEEDED( pd3dDevice->BeginScene() ) )
	{
		pDirector->RenderScene();
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

	DestroyAllVertexDeclarations();
	
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
    DXUTCreateWindow( L"Fireworks" );
    DXUTCreateDevice( true, 1024, 768 );

    // Start the render loop
    DXUTMainLoop();

    // TODO: Perform any application-level cleanup here

    return DXUTGetExitCode();
}
