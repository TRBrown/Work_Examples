#include "DXUT.h" 
#include "CameraManager.h"
#include "d3dStructures.h"

CameraManager* gCManager = 0;

CameraManager::CameraManager()
{
	// Default camera configuration
	m_v3CameraPosition = D3DXVECTOR3 (0.0f, -350.0f, 750.0f);
	m_v3LookAt = D3DXVECTOR3 (0,-350.0f,0);
	m_v3UpVec = D3DXVECTOR3 (0,1,0);
	m_v3ViewVector = m_v3LookAt - m_v3CameraPosition;
	D3DXVec3Normalize(&m_v3ViewVector, &m_v3ViewVector);
	m_fNearPlane = 1;
	m_fFarPlane = 10000000;
	m_fFovy = D3DX_PI / 2.0f;
	
	m_fAspectRatio = (float)1024 / 768;
	D3DXMatrixPerspectiveFovLH(&m_mProjectionMatrix, m_fFovy, m_fAspectRatio 
								, m_fNearPlane, m_fFarPlane); 
	
	D3DXMatrixLookAtLH(&m_mViewMatrix, &m_v3CameraPosition, &m_v3LookAt, &m_v3UpVec);

	m_mViewProjectionMatrix = m_mViewMatrix * m_mProjectionMatrix;

	m_bNeedUpdateView = false;
	m_bNeedUpdateProjection = false;
}

CameraManager* CameraManager::Instance()
{
	static CameraManager instance;
	return &instance;

}

CameraManager::~CameraManager()
{

}

void CameraManager::UpdateView()
{
	m_v3ViewVector = m_v3LookAt - m_v3CameraPosition;
	D3DXVec3Normalize(&m_v3ViewVector, &m_v3ViewVector);
	D3DXMatrixLookAtLH(&m_mViewMatrix, &m_v3CameraPosition, &m_v3LookAt, &m_v3UpVec);

	m_mViewProjectionMatrix = m_mViewMatrix * m_mProjectionMatrix;

	m_bNeedUpdateView = false;
	m_bNeedUpdateFrustum = true;
}

void CameraManager::UpdateProjection()
{
	D3DXMatrixPerspectiveFovLH(&m_mProjectionMatrix, m_fFovy, m_fAspectRatio 
		, m_fNearPlane, m_fFarPlane);

	m_mViewProjectionMatrix = m_mViewMatrix * m_mProjectionMatrix;

	m_bNeedUpdateProjection = false;
	m_bNeedUpdateFrustum = true;
}

void CameraManager::ZoomIn()
{
	D3DXVECTOR3 eyeNormal;
	D3DXVec3Normalize(&eyeNormal, &m_v3CameraPosition);
	m_v3CameraPosition = m_v3CameraPosition + eyeNormal;
	m_bNeedUpdateView = true;
}

void CameraManager::ZoomOut()
{
	D3DXVECTOR3 eyeNormal;
	D3DXVec3Normalize(&eyeNormal, &m_v3CameraPosition);
	m_v3CameraPosition = m_v3CameraPosition - eyeNormal;
	m_bNeedUpdateView = true;
}