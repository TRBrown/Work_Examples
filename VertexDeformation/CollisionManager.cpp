#include "DXUT.h" 
#include "CollisionManager.h"

CollisionManager::CollisionManager()
{

}

CollisionManager* CollisionManager::Instance()
{
	static CollisionManager instance;
	return &instance;

}

CollisionManager::~CollisionManager()

{

}

void CollisionManager::Initialise(CameraManager* CManager)
{
	m_pCmanager = CManager;
}

//Detects if the mouse pointer is over a object
CollisionResult CollisionManager::pointerCollision(int xPos, int yPos, ID3DXMesh* mesh, D3DXMATRIX* worldMatrix)
{
	CollisionResult collisionResult;

	//move pointer position out of 2d and into 3d 
	D3DXVECTOR3 v;
	v.x =  ( ( ( 2.0f * xPos ) / 800  ) - 1 ) / m_pCmanager->GetProjectionMatrix()->_11;
	v.y = -( ( ( 2.0f * yPos ) / 600 ) - 1 ) / m_pCmanager->GetProjectionMatrix()->_22;
	v.z =  1.0f;

	D3DXMATRIX m;
	D3DXVECTOR3 rayOrigin,rayDir;

	D3DXMatrixInverse( &m, NULL, m_pCmanager->GetViewMatrix());

	// Transform the screen space pick ray into 3D space
	rayDir.x  = v.x*m._11 + v.y*m._21 + v.z*m._31;
	rayDir.y  = v.x*m._12 + v.y*m._22 + v.z*m._32;
	rayDir.z  = v.x*m._13 + v.y*m._23 + v.z*m._33;
	rayOrigin.x = m._41;
	rayOrigin.y = m._42;
	rayOrigin.z = m._43;

	// Use inverse of world matrix
	D3DXMATRIX InvWorldMatrix;
	D3DXMatrixInverse(&InvWorldMatrix,NULL, worldMatrix);

	// Transform ray origin and direction by inv matrix
	D3DXVECTOR3 rayObjOrigin,rayObjDirection;

	D3DXVec3TransformCoord(&rayObjOrigin,&rayOrigin,&InvWorldMatrix);
	D3DXVec3TransformNormal(&rayObjDirection,&rayDir,&InvWorldMatrix);
	D3DXVec3Normalize(&rayObjDirection,&rayObjDirection);

	D3DXIntersect(mesh, &rayObjOrigin, &rayObjDirection, &collisionResult.result, 
					&collisionResult.faceCollision, NULL, NULL, 
					&collisionResult.distanceToCollision, NULL, NULL);

	return collisionResult;
}
