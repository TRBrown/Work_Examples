#include "DXUT.h" 
#include "Fountain.h"
#include "d3dUtils.h"

Fountain::Fountain(	IDirect3DDevice9*	pD3dDevice,
					XMLEvent			Event,
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
	D3DXMatrixTransformation(&m_mWorld, NULL ,NULL, NULL, NULL, NULL, &Event.v3Position);
	D3DXMatrixInverse(&m_mInvWorld,NULL, &m_mWorld);
	m_sEvent = Event;
}


Fountain::~Fountain()
{

}

void Fountain::initParticle(VertexParticle& out)
{
	// Generate about the origin.
	out.psPosition		= m_sEvent.v3Position;

	out.initialTime     = m_fTime;
	out.lifeTime        = GetRandomFloat(4.0f, 5.0f);
	out.colour			= m_sEvent.d3cColour;
	out.initialSize     = GetRandomFloat(32.0f, 50.0f);
	out.mass            = GetRandomFloat(0.8f, 1.2f);

	out.initialVelocity.x = GetRandomFloat(-30.5f, 40.5f);
	out.initialVelocity.y = GetRandomFloat(85.0f, 105.0f);
	out.initialVelocity.z = GetRandomFloat(-30.5f, 40.5f);
}
