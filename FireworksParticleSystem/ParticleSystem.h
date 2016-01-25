#ifndef PARTICLESYSTEM_H
#define PARTICLESYSTEM_H
#pragma warning(disable:4995)


#include "tinyxml/tinyxml.h"
#include "d3dx9.h" 
#include "d3d9.h"
#include "d3dStructures.h"
#include "Particle.h"
#include "d3dUtils.h"
#include <string>
#include <vector>
#include <list>

class ParticleSystem
{
private:

protected:

	
	//Technique handles

	D3DXHANDLE m_hTech;
	D3DXHANDLE m_hWVP;
	D3DXHANDLE m_hEyePosL;
	D3DXHANDLE m_hTex;
	D3DXHANDLE m_hTime;
	D3DXHANDLE m_hAccel;
	D3DXHANDLE m_hViewportHeight;
	D3DXHANDLE m_hColour;

	//D3D objects
	IDirect3DDevice9*	m_pD3dDevice;

	ID3DXEffect* 		m_pEffect;


	IDirect3DVertexBuffer9* m_pVertexBuffer;
	IDirect3DTexture9*		m_pTexture;
	
	std::vector<VertexParticle>	m_vParticles;
	std::vector<VertexParticle*>	m_vAliveParticles;
	std::vector<VertexParticle*>	m_vDeadParticles; 


	D3DXMATRIX				m_mWorld;
	D3DXMATRIX				m_mInvWorld;
	float					m_fTime;
	D3DXVECTOR3				m_v3Accel; // acceleration applied to the particles such as gravity
	int						m_iMaxNumParticles;
	float					m_iTimePerParticle;
	D3DCOLOR				m_d3cColour;

	bool				    m_bActive;

	//OLD

// 	// a pointer of type Particle; will be used to create an array of particles
// 	Particle* m_pParticles;
// 	// the number of particles this emitter has
// 	int m_iNumParticles;

public:

	bool				m_bFinished;

	ParticleSystem(	IDirect3DDevice9*	pD3dDevice,
					const std::string&	FXFileName, 
					const std::string&	TechniqueName, 
					const std::string&	TextureName, 
					const D3DXVECTOR3&	acceleration, 
					int					maxNumParticles,
					float				timePerParticle);
	
	~ParticleSystem();

	//ParticleSystem(const ParticleSystem &);

	// Access Methods
	float getTime();
	void  setTime(float t);

	void setWorldMatrix(const D3DXMATRIX& world);
	void addParticle();

	virtual void onLostDevice();
	virtual void onResetDevice();
	
	virtual void initParticle(VertexParticle& out) = 0;

	inline DWORD FLOAT_TO_DWORD( FLOAT f ) { return *((DWORD*)&f); }

	virtual void Update(float dt);

	virtual void Render();

};




#endif