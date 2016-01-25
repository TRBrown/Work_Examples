#include "DXUT.h" 
#include "WPawn.h"
#include "CustomModel.h"
#include "White.h"

WPawn::WPawn(IDirect3DDevice9* pd3dDevice) : White(pd3dDevice)
{
	m_sMeshFN = "Models\\chess3d\\WPawn.X";
	m_sNormalMapFN = "Models\\EmptyNormal.jpg";
	m_sFXFN = "Models\\chess.fx";

	m_v3Direction = D3DXVECTOR3(0.0f,0.0f,0.0f);

	D3DXMATRIX scale;
	D3DXMatrixScaling(&scale,0.25f,0.25f,0.25f);
	SetScale(scale);

	D3DXMatrixIdentity(&m_mRotationX);
	D3DXMatrixIdentity(&m_mRotationY);
	D3DXMatrixIdentity(&m_mRotationZ);
	D3DXMatrixIdentity(&m_mTranslation);
	D3DXMatrixIdentity(&m_mWorld);
	D3DXMatrixIdentity(&m_mView);
	D3DXMatrixIdentity(&m_mProjection);

	Loadfx();
	Loadmodel();

	m_iPieceType = 5;
	m_bslides = true;
}

WPawn::~WPawn()

{

}

/*void WPawn::AssignParameters(D3DLIGHT9& light1, D3DLIGHT9& light2, D3DXVECTOR3& eye, D3DMATRIX& view, D3DMATRIX& proj)
{
	SetLight1(&light1);
	SetLight2(&light2);
	m_mView = view;
	m_mProjection = proj;
	D3DXMATRIX mViewProjection = m_mView * m_mProjection;
	D3DXMATRIX mWorldViewProjection = m_mWorld * mViewProjection;
	D3DXMATRIX worldinverse;

	/*
	//ID3DXMesh* mesh = GetMesh();
	//IDirect3DVertexBuffer9* vb = 0;
	//mesh->GetVertexBuffer(&vb);
	

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
	D3DXVECTOR4 veceye( eye.x, eye.y, eye.z, 1.0f );

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
	m_pEffect->SetVector("vecEye",&veceye);
	m_hParameters = m_pEffect->EndParameterBlock();
}*/

void WPawn::update()
{

	D3DXMatrixTranslation(&m_mTranslation, m_v4Position.x, m_v4Position.y, m_v4Position.z);

}

