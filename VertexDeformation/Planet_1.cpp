#include "DXUT.h" 
#include "Planet_1.h"
#include "CustomModel.h"

Planet1::Planet1(IDirect3DDevice9* pd3dDevice) : CustomModel(pd3dDevice)
{
	m_sMeshFN = "Models\\planet_1.x";
	m_sNormalMapFN = "Models\\jupiter2_4k_normal.jpg";
	m_sFXFN = "Models\\planet_1.fx";
	m_pForcePosition = 0;	

	m_pCModelLocation = "Models\\";
	m_v3Direction = D3DXVECTOR3(0.0f,0.0f,0.0f);

	D3DXMatrixIdentity(&m_mScale);
	//D3DXMatrixScaling(&m_mScale, 0.25f,0.25f,0.25f); 
	D3DXMatrixIdentity(&m_mRotationX);
	D3DXMatrixIdentity(&m_mRotationY);
	D3DXMatrixIdentity(&m_mRotationZ);
	D3DXMatrixIdentity(&m_mTranslation);
	D3DXMatrixIdentity(&m_mWorld);
	D3DXMatrixIdentity(&m_mView);
	D3DXMatrixIdentity(&m_mProjection);

	Loadfx();
	Loadmodel();
}

Planet1::Planet1(IDirect3DDevice9* pd3dDevice, D3DXVECTOR4& forcePosition, float& forceRadius, float& forceStrength) : CustomModel(pd3dDevice)
{
	m_sMeshFN = "Models\\planet_1.x";
	m_sNormalMapFN = "Models\\jupiter2_4k_normal.jpg";
	m_sFXFN = "Models\\planet_1_def.fx";
	m_pForcePosition = &forcePosition;
	m_pForceRadius = &forceRadius;
	m_pForceStrength = &forceStrength;

	m_pCModelLocation = "Models\\";
	m_v3Direction = D3DXVECTOR3(0.0f,0.0f,0.0f);

	D3DXMatrixIdentity(&m_mScale);
	D3DXMatrixIdentity(&m_mRotationX);
	D3DXMatrixIdentity(&m_mRotationY);
	D3DXMatrixIdentity(&m_mRotationZ);
	D3DXMatrixIdentity(&m_mTranslation);
	D3DXMatrixIdentity(&m_mWorld);
	D3DXMatrixIdentity(&m_mView);
	D3DXMatrixIdentity(&m_mProjection);

	Loadfx();
	Loadmodel();
}

Planet1::Planet1(const Planet1 & rhs) : CustomModel(rhs)
{
	
	m_pForcePosition		=		rhs.m_pForcePosition;
	m_pForceRadius			=		rhs.m_pForceRadius;
	m_pForceRadius			=		rhs.m_pForceRadius;
	m_fRotationCounter		=		rhs.m_fRotationCounter;

}

Planet1::~Planet1()

{

}

void Planet1::AssignParameters(D3DLIGHT9& light1, D3DLIGHT9& light2, const D3DXVECTOR3 *eye, const D3DMATRIX *view, const D3DMATRIX *proj)
{
	m_pLight1 = &light1;
	m_pLight2 = &light2;
	m_mView = *view;
	m_mProjection = *proj;
	D3DXMATRIX mViewProjection = m_mView * m_mProjection;
	D3DXMATRIX mWorldViewProjection = m_mWorld * mViewProjection;
	D3DXMATRIX worldinverse;

	D3DXVECTOR4 forceDirection = m_v4Position - *m_pForcePosition;
	forceDirection.w = 0;

	/*
	ID3DXMesh* mesh = GetMesh();
	IDirect3DVertexBuffer9* vb = 0;
	mesh->GetVertexBuffer(&vb);
	*/

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
	m_pEffect->SetVector("forceDir",&forceDirection);
	m_pEffect->SetFloat("forceRadius",*m_pForceRadius);
	m_pEffect->SetFloat("forceStrength",*m_pForceStrength);
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

void Planet1::update()
{
	if (m_fRotationCounter >= (D3DX_PI * 2)) 
	{
		m_fRotationCounter = 0.2;
	}
	else
	{
		m_fRotationCounter += 0.0872664626;
	}

	D3DXMatrixTranslation(&m_mTranslation, m_v4Position.x, m_v4Position.y, m_v4Position.z);

	//Matrix
	D3DMATRIX RotationA = m_mRotationX * m_mRotationY * m_mRotationZ;
	m_mWorld = m_mScale * RotationA * m_mTranslation;


}