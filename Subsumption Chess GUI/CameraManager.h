#ifndef CAMERAMANAGER_H
#define CAMERAMANAGER_H
#pragma warning(disable:4995)

#include "d3dx9.h" 
#include "d3d9.h"
//#include <string>
//#include <vector>




class CameraManager
{
private:
	
	// Perspective projection parameters
	float m_fFovy;
	float m_fAspectRatio;
	float m_fNearPlane;
	float m_fFarPlane;

	D3DXMATRIX m_mViewMatrix;
	D3DXMATRIX m_mProjectionMatrix;

	// Position and target
	D3DXVECTOR3 m_v3CameraPosition;
	D3DXVECTOR3 m_v3LookAt;
	D3DXVECTOR3 m_v3UpVec;
	D3DXVECTOR3 m_v3ViewVector;

	CameraManager();

	CameraManager(const CameraManager &);

protected:

	// Matrices and updates
	bool m_bNeedUpdateView;
	bool m_bNeedUpdateProjection;
	bool m_bNeedUpdateFrustum;

public:

	static CameraManager* Instance();

	~CameraManager();

	void UpdateView();
	void UpdateProjection();
	void ZoomIn();
	void ZoomOut();

	//Accessors
	float GetFovy()const{return m_fFovy;}
	void SetFovy(const float fovy){m_fFovy = fovy; m_bNeedUpdateProjection = true;}

	float GetAspectRatio()const{return m_fAspectRatio;}
	void SetAspectRatio(const float ar){m_fAspectRatio = ar; m_bNeedUpdateProjection = true;}

	float GetNearPlane()const{return m_fNearPlane;}
	void SetNearPlane(const float np){m_fNearPlane = np; m_bNeedUpdateProjection = true;}

	float GetFarPlane()const{return m_fFarPlane;}
	void SetFarPlane(const float fp){m_fFarPlane = fp; m_bNeedUpdateProjection = true;}

	const D3DXVECTOR3* GetCameraPosition()const{return &m_v3CameraPosition;}
	void SetCameraPosition(const D3DXVECTOR3 cp){m_v3CameraPosition = cp; m_bNeedUpdateView = true;}

	D3DXVECTOR3 GetLookAt()const{return m_v3LookAt;}
	void SetLookAt(const D3DXVECTOR3 la){m_v3LookAt = la; m_bNeedUpdateView = true;}

	D3DXVECTOR3 GetUpVec()const{return m_v3UpVec;}
	void SetUpVec(const D3DXVECTOR3 uv){m_v3UpVec = uv; m_bNeedUpdateView = true;}

	D3DXVECTOR3 GetViewVector()const{return m_v3ViewVector;}

	//Gets Matrix's and updates them on change
	D3DXMATRIX* GetViewMatrix()
	{
		if (m_bNeedUpdateView) UpdateView();
		return &m_mViewMatrix;
	}
	
	D3DXMATRIX* GetProjectionMatrix()
	{
		if (m_bNeedUpdateProjection) UpdateProjection();
		return &m_mProjectionMatrix;
	}

	//Checks to see if the camera is pointing directly up or down to avoid
	//the camera Rollover as it flips to retain its up direction.
	bool OrbitAboveCheck() 
	{
		if (m_v3ViewVector.y > -0.9400000)return true;
		else return false;
	
	}
	bool OrbitBelowCheck() 
	{
		if (m_v3ViewVector.y < 0.9400000)return true;
		else return false;

	}

};




#endif

