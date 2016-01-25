#ifndef CUSTOM_MODEL
#define CUSTOM_MODEL
#pragma warning(disable:4995)

#include "d3dx9.h" 
#include "d3d9.h"
#include <string>
#include <malloc.h>



class CustomModel 
{
private:

	struct CUSTOMVERTEX
	{
		D3DXVECTOR3 POSITION;
		D3DXVECTOR3 TANGENT;
		D3DXVECTOR3 BINORMAL;
		D3DXVECTOR3 NORMAL;
		D3DXVECTOR2 TEXCOORD;
	};

	struct FaceVertexs
	{
		D3DXVECTOR3 vert1;
		D3DXVECTOR3 vert2;
		D3DXVECTOR3 vert3;

	};

	struct NMapVertex
	{
		D3DXVECTOR3 pos;
		D3DXVECTOR3 tangent;
		D3DXVECTOR3 binormal;
		D3DXVECTOR3 normal;
		D3DXVECTOR2 tex0;

		static IDirect3DVertexDeclaration9* Decl;
	};

	ID3DXMesh*				m_pMesh;
	DWORD					m_dNumMaterials;
	D3DMATERIAL9*           m_pMeshMaterials; // Materials for our mesh
	LPDIRECT3DTEXTURE9*     m_pMeshTextures; // Textures for our mesh
	IDirect3DTexture9*      m_pNormalmapTex;
	wchar_t*				m_pWMeshFN;
	wchar_t*				m_pWNormalMapFN;
	wchar_t*				m_pWFXFN;


protected:

	char*					m_pCModelLocation;
	ID3DXEffect* 			m_pEffect; // pointer to effect object
	D3DXHANDLE				m_hParameters;
	D3DLIGHT9*				m_pLight1;
	D3DLIGHT9*				m_pLight2;
	FaceVertexs*			m_pFaces;
	
	D3DXVECTOR4				m_v4Position;
	D3DXVECTOR3				m_v3Direction;
	
	D3DXMATRIX				m_mScale;
	D3DXMATRIX				m_mRotationX;
	D3DXMATRIX				m_mRotationY;
	D3DXMATRIX				m_mRotationZ;
	D3DXMATRIX				m_mTranslation;
	D3DXMATRIX				m_mWorld;
	D3DXMATRIX				m_mView;
	D3DXMATRIX				m_mProjection;

public:

	bool					m_bError;
	bool					m_bVisible;
	HRESULT					hr;
	std::string				m_sMeshFN;
	std::string				m_sNormalMapFN;
	std::string				m_sFXFN;
	IDirect3DDevice9*		m_pD3dDevice;

	
	CustomModel(std::string strMesh, std::string strFX, IDirect3DDevice9* pd3dDevice);
	CustomModel(std::string strMesh, IDirect3DDevice9* pd3dDevice);
	CustomModel(IDirect3DDevice9* pd3dDevice);

	CustomModel(const CustomModel &);

	virtual ~CustomModel();

	D3DLIGHT9*	GetLight1() const	{return m_pLight1;}
	D3DLIGHT9*	GetLight2() const	{return m_pLight2;}
	ID3DXEffect*	GetEffect() const	{return m_pEffect;}
	ID3DXMesh*	GetMesh() const	{return m_pMesh;}
	D3DXVECTOR3 GetFaceVertex(DWORD faceNumber) const {return m_pFaces[faceNumber].vert1;}
	D3DXVECTOR4	GetPosition() const	{return m_v4Position;}

	void	SetLight1(D3DLIGHT9* val){m_pLight1 = val;}
	void	SetLight2(D3DLIGHT9* val){m_pLight2 = val;}

	void	SetPosition(const D3DXVECTOR4 val){m_v4Position = val;}
	void	SetDirection(const D3DXVECTOR3 val){m_v3Direction = val;}

	void	SetScale(const D3DXMATRIX val){m_mScale = val;}
	void	SetRotationX(const D3DXMATRIX val){m_mRotationX = val;}
	void	SetRotationY(const D3DXMATRIX val){m_mRotationY = val;}
	void	SetRotationZ(const D3DXMATRIX val){m_mRotationZ = val;}
	void	SetTranslation(const D3DXMATRIX val){m_mTranslation = val;}
	void	SetWorld(const D3DXMATRIX val){m_mWorld = val;}
	D3DXMATRIX*	GetWorldMatrix() {return &m_mWorld;}
	void	SetView(const D3DXMATRIX val){m_mView = val;}
	void	SetProjection(const D3DXMATRIX val){m_mProjection = val;}

	virtual void Loadmodel();
	virtual void Loadfx();
	virtual void AssignParameters(D3DLIGHT9& light1, D3DLIGHT9& light2, const D3DXVECTOR3 *eye, const D3DMATRIX *view, const D3DMATRIX *proj);
	virtual void update();
	virtual void render();
	bool Error()const{return m_bError;} 

};




#endif

