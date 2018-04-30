#ifndef COLLISIONMANAGER_H
#define COLLISIONMANAGER_H
#pragma warning(disable:4995)

#include "d3dx9.h" 
#include "d3d9.h"
#include "CameraManager.h"
//#include "FontManager.h"
//#include "Custom_Math.h"
//#include <string>
//#include <vector>

typedef struct CollisionResult
{
	public:

	BOOL result;
	float distanceToCollision;
	DWORD faceCollision;

} CollisionResult, *LPCollisionResult;

class CollisionManager
{
private:

	CameraManager* m_pCmanager;

	CollisionManager();

	CollisionManager(const CollisionManager &);

protected:


public:

	static CollisionManager* Instance();

	~CollisionManager();

	void Initialise(CameraManager* CManager);

	CollisionResult pointerCollision(int xPos, int yPos, ID3DXMesh* mesh, D3DXMATRIX* worldMatrix);		  
	//Accessors


};




#endif

