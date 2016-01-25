#include "DXUT.h" 
#include "Explosion.h"
#include "d3dUtils.h"

Explosion::Explosion(	IDirect3DDevice9*	pD3dDevice,
	const std::string&	FXFileName, 
	const std::string&	TechniqueName, 
	const std::string&	TextureName, 
	const D3DXVECTOR3&	acceleration, 
	int					maxNumParticles,
	float				timePerParticle) : ParticleSystem (	pD3dDevice,
															FXFileName,
															TechniqueName,
															TextureName,
															acceleration,
															maxNumParticles,
															timePerParticle)
{
	m_pD3dDevice = pD3dDevice;
	D3DXMatrixIdentity(&m_mWorld);
	D3DXMatrixIdentity(&m_mInvWorld);
	m_d3cColour = NULL;
	m_v3Position = D3DXVECTOR3();

	Deactivate();
}


Explosion::~Explosion()
{

}

void Explosion::Activate(D3DXVECTOR3 &position, D3DCOLOR &colour)
{
	
	m_v3Position = position;
	m_d3cColour = colour;

	D3DXMatrixTransformation(&m_mWorld, NULL ,NULL, NULL, NULL, NULL, &m_v3Position);
	D3DXMatrixInverse(&m_mInvWorld,NULL, &m_mWorld);
	
	for(int i = 0; i < m_iMaxNumParticles; ++i)
	{

		m_vDeadParticles.push_back(&m_vParticles[i]);
		addParticle();
	}

	m_bActive = true;
}

void Explosion::initParticle(VertexParticle& out)
{
	// Generate about the origin.
	out.psPosition		= m_v3Position;

	out.initialTime     = m_fTime;
	out.lifeTime        = GetRandomFloat(0.5f, 1.0f);
	out.colour			= m_d3cColour;
	out.initialSize     = GetRandomFloat(32.0f, 50.0f);
	out.mass            = GetRandomFloat(0.8f, 1.2f);

	out.initialVelocity.x = GetRandomFloat(-100.0f, 100.0f);
	out.initialVelocity.y = GetRandomFloat(-100.0f, 100.0f);
	out.initialVelocity.z = GetRandomFloat(-100.0f, 100.0f);
}

void Explosion::Update(float dt)
{
	if (m_bActive)
	{
		m_fTime += dt;

		// Rebuild the dead and alive list.  Note that resize(0) does
		// not deallocate memory (i.e., the capacity of the vector does
		// not change).
		m_vDeadParticles.resize(0);
		m_vAliveParticles.resize(0);

		// For each particle.
		for(int i = 0; i < m_iMaxNumParticles; ++i)
		{
			// Is the particle dead?
			if( (m_fTime - m_vParticles[i].initialTime) > m_vParticles[i].lifeTime)
			{
				m_vDeadParticles.push_back(&m_vParticles[i]);
			}
			else
			{
				m_vAliveParticles.push_back(&m_vParticles[i]);
			}
		}


		//Allow the particle system to be deleted when all active particles are dead
		if (m_bActive && m_vAliveParticles.empty()) 
		{
			m_bFinished = true;
		}
	}

}