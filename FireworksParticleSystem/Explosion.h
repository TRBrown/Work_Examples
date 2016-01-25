#ifndef EXPLOSION_H
#define EXPLOSION_H
#pragma warning(disable:4995)

#include "ParticleSystem.h"
#include "tinyxml/tinyxml.h"
#include "d3dx9.h" 
#include "d3d9.h"
#include "d3dStructures.h"
#include <d3dx9tex.h>
#include <string>
#include <list>

#include "Particle.h"
#include "d3dUtils.h"

class Explosion : public ParticleSystem
{

private:


	D3DCOLOR			m_d3cColour;
	D3DXVECTOR3			m_v3Position; //The explosion position


protected:

public:

	Explosion(	IDirect3DDevice9*	pD3dDevice,
		const std::string&	FXFileName, 
		const std::string&	TechniqueName, 
		const std::string&	TextureName, 
		const D3DXVECTOR3&	acceleration, 
		int					maxNumParticles,
		float				timePerParticle);

	~Explosion();

	//Explosion (const Explosion &);

	bool GetState()const{return m_bActive;} 
	void Activate(D3DXVECTOR3 &position, D3DCOLOR &colour);
	void Deactivate(){m_bActive = false; m_iTimePerParticle = 0.0f;}

	void initParticle(VertexParticle& out);

	virtual void Update(float dt);

};




#endif