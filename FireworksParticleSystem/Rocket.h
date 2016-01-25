#ifndef ROCKET_H
#define ROCKET_H
#pragma warning(disable:4995)

#include "ParticleSystem.h"
#include "Explosion.h"
#include "tinyxml/tinyxml.h"
#include "d3dx9.h" 
#include "d3d9.h"
#include "d3dStructures.h"
#include <d3dx9tex.h>
#include <string>
#include <list>

#include "Particle.h"
#include "d3dUtils.h"

class Rocket : public ParticleSystem
{

private:

	//XML Loaded Event data
//	D3DCOLOR			m_d3cColour;
//	D3DXVECTOR3			m_v3Position; //start position of rocket
//	D3DXVECTOR3			m_v3Velocity; // only used for rocket (defines movement of the rocket prior to exploding)
//	bool				m_bActive;

//	std::string			m_sType; //A string donating the type of event
//	int					m_iStart;  // The starting time of the fountain
//	int					m_iDuration;  // The burn duration
	
	XMLEvent			m_sEvent;

	bool				m_bExplode;
	Explosion*			m_pExplosion;

	D3DXVECTOR3	m_v3CurrentPosition;

protected:

public:


	Rocket(	IDirect3DDevice9*	pD3dDevice,
			XMLEvent			Event,
			const std::string&	FXFileName, 
			const std::string&	TechniqueName, 
			const std::string&	TextureName, 
			const D3DXVECTOR3&	acceleration, 
			int					maxNumParticles,
			float				timePerParticle);

	~Rocket();

	//Rocket (const Rocket &);

	int GetStart()const{return m_sEvent.iStart;}
	int GetEnd()const{return m_sEvent.iDuration;}

	bool GetState()const{return m_bActive;} 
	void Activate(){m_bActive = true;}
	void Deactivate(){	m_bActive = false; 
						m_iTimePerParticle = 0.0f; 
						m_pExplosion->Activate(m_sEvent.v3Position, m_sEvent.d3cColour);}

	void initParticle(VertexParticle& out);

	virtual void Update(float dt);

	virtual void Render();

};




#endif