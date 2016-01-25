#include "DXUT.h" 
#include "CustomModel.h"
IDirect3DVertexDeclaration9* CustomModel::NMapVertex::Decl  = 0;

CustomModel::CustomModel(std::string strMesh, std::string strFX, IDirect3DDevice9* pd3dDevice)
{
	m_sMeshFN =  strMesh;
	m_sFXFN =  strFX;
	m_pD3dDevice = pd3dDevice;
	m_bError = false;

	Loadmodel();
	Loadfx();

}

CustomModel::CustomModel(std::string strMesh, IDirect3DDevice9* pd3dDevice)
{
	m_sMeshFN =  strMesh;
	m_pD3dDevice = pd3dDevice;
	m_bError = false;

	Loadmodel();

}

CustomModel::CustomModel(IDirect3DDevice9* pd3dDevice)
{
	m_pD3dDevice = pd3dDevice;
	m_bError = false;

	//Init Vertex Declarations
	D3DVERTEXELEMENT9 NMapVertexElements[] = 
	{
		{0, 0,  D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},
		{0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TANGENT, 0},
		{0, 24, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BINORMAL, 0},
		{0, 36, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0},
		{0, 48, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0},
		D3DDECL_END()
	};	
	hr = m_pD3dDevice->CreateVertexDeclaration(NMapVertexElements, &NMapVertex::Decl);

}

CustomModel::CustomModel(const CustomModel & rhs)
{
	m_sMeshFN		=		rhs.m_sMeshFN;
	m_sFXFN			=		rhs.m_sFXFN;
	m_pD3dDevice	=		rhs.m_pD3dDevice;
	m_bError		=		false;

	Loadmodel();
	Loadfx();

	m_pLight1		=		rhs.m_pLight1;
	m_mScale		=		rhs.m_mScale;
	m_mRotationX	=		rhs.m_mRotationX;
	m_mRotationY	=		rhs.m_mRotationY;
	m_mRotationZ	=		rhs.m_mRotationZ;
	m_mTranslation	=		rhs.m_mTranslation;
	m_mWorld		=		rhs.m_mWorld;
	m_mView			=		rhs.m_mView;
	m_mProjection	=		rhs.m_mProjection;

}

CustomModel::~CustomModel()

{

	m_pMesh->Release();
	m_pMesh = 0;

	m_pNormalmapTex->Release();
	m_pNormalmapTex = 0;

	for( UINT i = 0; i < m_dNumMaterials; i++ ) 
	
	SAFE_RELEASE( m_pMeshTextures[i] );
	SAFE_DELETE_ARRAY( m_pMeshTextures );
	m_pMeshTextures = 0;
	SAFE_DELETE_ARRAY( m_pMeshMaterials );
	m_pMeshMaterials = 0;

	if( m_pEffect != 0 )
		m_pEffect->Release();	
	m_pEffect = 0;
}


void CustomModel::Loadmodel()
{
	
	IDirect3DTexture9*          m_pDefaultTex;   // A default texture
	ID3DXBuffer*				m_pMaterialBuffer;
	m_dNumMaterials = NULL;

	ID3DXMesh* tempMesh = 0;
	ID3DXMesh* clonedmesh = 0;

	D3DVERTEXELEMENT9 elems[MAX_FVF_DECL_SIZE];
	UINT numElems = 0;

	ID3DXBuffer* cleanbuffer = 0;

	//Load Normal Map
	int size = MultiByteToWideChar(CP_ACP, 0, m_sNormalMapFN.c_str(), -1, 0, 0);
	m_pWNormalMapFN = (wchar_t*)malloc(size * sizeof(wchar_t));
	MultiByteToWideChar(CP_ACP, 0, m_sNormalMapFN.c_str(), -1, m_pWNormalMapFN, size);

	if( FAILED( hr = D3DXCreateTextureFromFile(m_pD3dDevice, m_pWNormalMapFN, &m_pNormalmapTex)))
	{
		m_bError = true;
	}

	//Creates a default texture of white
	hr = m_pD3dDevice->CreateTexture( 1, 1, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &m_pDefaultTex, NULL );
	D3DLOCKED_RECT lr;
	m_pDefaultTex->LockRect( 0, &lr, NULL, 0 );
	*( LPDWORD )lr.pBits = D3DCOLOR_RGBA( 255, 255, 255, 255 );
	m_pDefaultTex->UnlockRect( 0 );

	//Load Mesh


	size = MultiByteToWideChar(CP_ACP, 0, m_sMeshFN.c_str(), -1, 0, 0);
	m_pWMeshFN = (wchar_t*)malloc(size * sizeof(wchar_t));
	MultiByteToWideChar(CP_ACP, 0, m_sMeshFN.c_str(), -1, m_pWMeshFN, size);

	//TNB basis Vectors

	//Load Mesh into temp
	if( FAILED( hr = D3DXLoadMeshFromX(m_pWMeshFN, D3DXMESH_SYSTEMMEM,
		m_pD3dDevice, NULL,
		&m_pMaterialBuffer,NULL, &m_dNumMaterials,
		&tempMesh)) )

	{
		const WCHAR* test = DXGetErrorString(hr);
		m_bError = true;
	}


	hr = NMapVertex::Decl->GetDeclaration(elems, &numElems);

	hr = tempMesh->CloneMesh(D3DXMESH_MANAGED, elems, m_pD3dDevice, &clonedmesh);

	//Mesh Clean

	DWORD* adjacency = new DWORD[3 * clonedmesh->GetNumFaces()];
	DWORD* adjacencyOut = new DWORD[3 * clonedmesh->GetNumFaces()];

	hr = clonedmesh->GenerateAdjacency( 1e-4f, adjacency );

	tempMesh->Release();
	tempMesh = 0;

	//tempmesh is now cleaned :/

	hr = D3DXCleanMesh( D3DXCLEAN_BOWTIES, clonedmesh, adjacency, &tempMesh, adjacencyOut, &cleanbuffer );
	//End Mesh clean

	hr = D3DXComputeTangentFrameEx(tempMesh,
		D3DDECLUSAGE_TEXCOORD, 0,
		D3DDECLUSAGE_BINORMAL, 0,
		D3DDECLUSAGE_TANGENT, 0,
		D3DDECLUSAGE_NORMAL, 0,
		0,
		adjacencyOut,
		0.01f, 0.25f, 0.01f,
		&m_pMesh,
		0);

	D3DXMATERIAL* d3dxMaterials = (D3DXMATERIAL*)m_pMaterialBuffer->GetBufferPointer();

	m_pMeshMaterials = new D3DMATERIAL9[m_dNumMaterials];
	m_pMeshTextures  = new LPDIRECT3DTEXTURE9[m_dNumMaterials];

	//load the materials and textures into the texture buffer
	for (DWORD i=0; i<m_dNumMaterials; i++)
	{

		// Copy the material
		m_pMeshMaterials[i] = d3dxMaterials[i].MatD3D;

		// Create the texture if it exists - it may not
		m_pMeshTextures[i] = NULL;
		// Get a path to the texture

		if( d3dxMaterials[i].pTextureFilename != NULL )
		{
			char fnTemp[MAX_PATH];
			strcpy_s(fnTemp, m_pCModelLocation);
			strcat_s(fnTemp,d3dxMaterials[i].pTextureFilename);
			WCHAR wszBuf[MAX_PATH];
			MultiByteToWideChar( CP_ACP, 0, fnTemp, -1, wszBuf, MAX_PATH );
			wszBuf[MAX_PATH - 1] = L'\0';

			// Load the texture
			D3DXCreateTextureFromFile( m_pD3dDevice, wszBuf, &m_pMeshTextures[i] );
		}
		else
		{
			// Use the default texture
			m_pMeshTextures[i] = m_pDefaultTex;
			m_pMeshTextures[i]->AddRef();
		}


	}

	//
	DWORD numFaces = m_pMesh->GetNumFaces();

	m_pFaces = new FaceVertexs[numFaces];

	CUSTOMVERTEX *pVertices;
	WORD* pIndices;

	m_pMesh->LockIndexBuffer(D3DLOCK_READONLY, (VOID**)&pIndices);
	m_pMesh->LockVertexBuffer(D3DLOCK_READONLY,(VOID**)&pVertices);

	for ( DWORD face = 0; face < numFaces ; face++ )
	{
		m_pFaces[face].vert1 = pVertices[pIndices[3*face+0]].POSITION;
		m_pFaces[face].vert2 = pVertices[pIndices[3*face+1]].POSITION;
		m_pFaces[face].vert3 = pVertices[pIndices[3*face+2]].POSITION;
	}

	m_pMesh->UnlockVertexBuffer();
	m_pMesh->UnlockIndexBuffer();

	//Object cleanup
	m_pMaterialBuffer->Release();
	m_pMaterialBuffer = 0;

	m_pDefaultTex->Release();
	m_pDefaultTex = 0;

	tempMesh->Release();
	tempMesh = 0;

	clonedmesh->Release();
	clonedmesh = 0;

	if(cleanbuffer != 0)
	{
		cleanbuffer->Release();
		cleanbuffer = 0;
	}

	delete [] adjacency;
	delete [] adjacencyOut;

	if(NMapVertex::Decl != 0)
	{
		NMapVertex::Decl->Release();
		NMapVertex::Decl = 0;
	}
};

void CustomModel::Loadfx()
{
    int size = MultiByteToWideChar(CP_ACP, 0, m_sFXFN.c_str(), -1, 0, 0);
	m_pWFXFN = (wchar_t*)_malloca(size * sizeof(wchar_t));
	MultiByteToWideChar(CP_ACP, 0, m_sFXFN.c_str(), -1, m_pWFXFN, size);
	
	if( FAILED( hr = D3DXCreateEffectFromFile( m_pD3dDevice, m_pWFXFN, NULL, NULL, 
	D3DXSHADER_DEBUG, NULL, &m_pEffect, NULL ) ) )
	{
		m_bError = true;
	}

};



void CustomModel::AssignParameters(D3DLIGHT9& light1, D3DLIGHT9& light2, const D3DXVECTOR3 *eye, const D3DMATRIX *view, const D3DMATRIX *proj)
{
	m_pLight1 = &light1;
	m_pLight2 = &light2;
	m_mView = *view;
	m_mProjection = *proj;
	D3DXMATRIX mViewProjection = m_mView * m_mProjection;
	D3DXMATRIX mWorldViewProjection = m_mWorld * mViewProjection;
	D3DXMATRIX worldinverse;

	//Effect parameter Block
	m_pEffect->BeginParameterBlock();
	D3DXVECTOR4 diff1v4( m_pLight1->Diffuse.r, m_pLight1->Diffuse.g, m_pLight1->Diffuse.b, m_pLight1->Diffuse.a );
	D3DXVECTOR4 spec1v4( m_pLight1->Specular.r, m_pLight1->Specular.g, m_pLight1->Specular.b, m_pLight1->Specular.a );
	D3DXVECTOR4 amb1v4( m_pLight1->Ambient.r, m_pLight1->Ambient.g, m_pLight1->Ambient.b, m_pLight1->Ambient.a ); 
	D3DXVECTOR4 vec1Pos( m_pLight1->Position.x, m_pLight1->Position.y, m_pLight1->Position.z, 0.0f );
	
	D3DXVECTOR4 diff2v4( m_pLight2->Diffuse.r, m_pLight2->Diffuse.g, m_pLight2->Diffuse.b, m_pLight2->Diffuse.a );
	D3DXVECTOR4 spec2v4( m_pLight2->Specular.r, m_pLight2->Specular.g, m_pLight2->Specular.b, m_pLight2->Specular.a );
	D3DXVECTOR4 amb2v4( m_pLight2->Ambient.r, m_pLight2->Ambient.g, m_pLight2->Ambient.b, m_pLight2->Ambient.a ); 
	D3DXVECTOR4 vec2Dir( m_pLight2->Direction.x, m_pLight2->Direction.y, m_pLight2->Direction.z, 0.0f );	
		
	D3DXMatrixInverse(&worldinverse, 0, &m_mWorld);
	D3DXVECTOR4 eyev4( eye->x, eye->y, eye->z, 1.0f );
	
	m_pEffect->SetMatrix( "wMat", &m_mWorld);
	m_pEffect->SetMatrix( "wvpMat", &mWorldViewProjection);
	m_pEffect->SetMatrix( "wInvMat", &worldinverse);
	m_pEffect->SetVector("vecLight1Pos",&vec1Pos);
	m_pEffect->SetVector("vecLight1Amb",&amb1v4);
	m_pEffect->SetVector("vecLight1Spec",&spec1v4);
	m_pEffect->SetVector("vecLight1Diff",&diff1v4);

	m_pEffect->SetVector("vecLight2Dir",&vec2Dir);
	m_pEffect->SetVector("vecLight2Amb",&amb2v4);
	m_pEffect->SetVector("vecLight2Spec",&spec2v4);
	m_pEffect->SetVector("vecLight2Diff",&diff2v4);
	m_pEffect->SetVector("vecEye",&eyev4);
	m_hParameters = m_pEffect->EndParameterBlock();
}

void CustomModel::update()
{

	D3DXMatrixTranslation(&m_mTranslation, m_v4Position.x, m_v4Position.y, m_v4Position.z);

	//Matrix
	D3DMATRIX RotationA = m_mRotationX * m_mRotationY * m_mRotationZ;
	m_mWorld = m_mScale * RotationA * m_mTranslation;

}
void CustomModel::render()
{
	unsigned int numOfPasses = 1;
	unsigned int passIndex;
		
	if( m_pEffect != NULL )
	{
		//Technique binds
		D3DXHANDLE hTechnique = m_pEffect->GetTechniqueByName( "effect" );
		m_pEffect->SetTechnique( hTechnique );
		

		m_pEffect->ApplyParameterBlock( m_hParameters );
		
		//Begin Effect/Shader
		///////////////////////////////////
		m_pEffect->Begin( &numOfPasses, 0 );	// start an active technique
		// loop through all render passes in technique
		for(passIndex = 0; passIndex < numOfPasses; ++passIndex)
		{
			m_pEffect->BeginPass(passIndex);	// begin a pass, within active technique
			for (DWORD i=0; i<m_dNumMaterials; i++)
			{
			   // Set the material and texture for this subset
			  m_pD3dDevice->SetMaterial(&m_pMeshMaterials[i]);
			  m_pD3dDevice->SetTexture(0, m_pMeshTextures[i]);
			  m_pD3dDevice->SetTexture(1, m_pNormalmapTex);

			  D3DXVECTOR4 matDiff4( m_pMeshMaterials[i].Diffuse.r, m_pMeshMaterials[i].Diffuse.g, m_pMeshMaterials[i].Diffuse.b, m_pMeshMaterials[i].Diffuse.a );
			  m_pEffect->SetVector("matDiff",&matDiff4);
			  D3DXVECTOR4 matAmb4( m_pMeshMaterials[i].Ambient.r, m_pMeshMaterials[i].Ambient.g, m_pMeshMaterials[i].Ambient.b, m_pMeshMaterials[i].Ambient.a );
			  m_pEffect->SetVector("matAmb",&matAmb4);
			  D3DXVECTOR4 matSpec4( m_pMeshMaterials[i].Specular.r, m_pMeshMaterials[i].Specular.g, m_pMeshMaterials[i].Specular.b, m_pMeshMaterials[i].Specular.a );
			  m_pEffect->SetVector("matSpec",&matSpec4);
			  D3DXVECTOR4 matEmiss4( m_pMeshMaterials[i].Emissive.r, m_pMeshMaterials[i].Emissive.g, m_pMeshMaterials[i].Emissive.b, m_pMeshMaterials[i].Emissive.a );
			  m_pEffect->SetVector("matEmiss",&matEmiss4);
			  float matPower = m_pMeshMaterials[i].Power;
			  m_pEffect->SetFloat("matPower",matPower);
			  m_pEffect->CommitChanges();    
			  // Draw the mesh subset
			  m_pMesh->DrawSubset( i );
			}
			m_pEffect->EndPass(); // signal end of rendering current pass 
		}
	m_pEffect->End();  // signal end of rendering current technique
	m_pEffect->DeleteParameterBlock( m_hParameters );
	}

};