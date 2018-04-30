#include "DXUT.h" 
#include "Rocket.h"
#include "CameraManager.h"
#include "d3dStructures.h"


Rocket::Rocket(	IDirect3DDevice9*	pD3dDevice,
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

	m_pExplosion = new Explosion(	m_pD3dDevice,
									"Effects\\Particle.fx",
									"Fountain",
									"Textures\\particle.png",
									D3DXVECTOR3(-3.0f, -9.8f, 0.0f),
									500,
									0.0f);

}


Rocket::~Rocket()
{

	delete m_pExplosion;

}

void Rocket::initParticle(VertexParticle& out)
{
		// Generate about the origin.
		out.psPosition		= m_sEvent.v3Position;

		out.initialTime     = m_fTime;
		out.lifeTime        = GetRandomFloat(0.0f, 0.5f);
		out.colour			= m_sEvent.d3cColour;
		out.initialSize     = GetRandomFloat(32.0f, 50.0f);
		out.mass            = GetRandomFloat(0.8f, 1.2f);

		out.initialVelocity.x = GetRandomFloat(-40.5f, 40.5f);
		out.initialVelocity.y = GetRandomFloat(-65.0f, -85.0f);
		out.initialVelocity.z = GetRandomFloat(-40.5f, 40.5f);

		out.initialVelocity += m_sEvent.v3Velocity;
}

void Rocket::Update(float dt)
{
	m_fTime += dt;

	m_sEvent.v3Position = m_sEvent.v3Position + (m_sEvent.v3Velocity * m_fTime);

	m_pExplosion->Update(dt);

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


	// A negative or zero mTimePerParticle value denotes
	// not to emit any particles.
	if( m_iTimePerParticle > 0.0f )
	{
		// Emit particles.
		static float timeAccum = 0.0f;
		timeAccum += dt;
		while( timeAccum >= m_iTimePerParticle )
		{
			addParticle();
			timeAccum -= m_iTimePerParticle;
		}
	}

	//Allow the particle system to be deleted when all active particles are dead
	if (!m_bActive && m_vAliveParticles.empty() && m_pExplosion->m_bFinished) 
	{
		m_bFinished = true;
	}

}

void Rocket::Render()
{
	// Get camera position relative to world space system and make it 
	// relative to the particle system's local system.
	D3DXVECTOR3 eyePosW = gCManager->GetCameraPosition();
	D3DXVECTOR3 eyePosL;
	D3DXVec3TransformCoord(&eyePosL, &eyePosW, &m_mInvWorld);

	// Set FX parameters.
	HR(m_pEffect->SetValue(m_hEyePosL, &eyePosL, sizeof(D3DXVECTOR3)));
	HR(m_pEffect->SetFloat(m_hTime, m_fTime));
	HR(m_pEffect->SetMatrix(m_hWVP, &(m_mWorld * *gCManager->GetViewProjectionMatrix())));

	// Point sprite sizes are given in pixels.  So if the viewport size 
	// is changed, then more or less pixels become available, which alters
	// the perceived size of the particles.  For example, if the viewport
	// is 32x32, then a 32x32 sprite covers the entire viewport!  But if
	// the viewport is 1024x1024, then a 32x32 sprite only covers a small
	// portion of the viewport.  Thus, we scale the particle's
	// size by the viewport height to keep them in proportion to the 
	// viewport dimensions.
	D3DVIEWPORT9 d3dviewport; 
	HR(m_pD3dDevice->GetViewport(&d3dviewport));

	HR(m_pEffect->SetInt(m_hViewportHeight, d3dviewport.Height));

	UINT numPasses = 0;
	HR(m_pEffect->Begin(&numPasses, 0));
	HR(m_pEffect->BeginPass(0));

	HR(m_pD3dDevice->SetStreamSource(0, m_pVertexBuffer, 0, sizeof(VertexParticle)));
	HR(m_pD3dDevice->SetVertexDeclaration(VertexParticle::Decl));


	// Initial lock of VB for writing.
	VertexParticle* p = 0;
	HR(m_pVertexBuffer->Lock(0, 0, (void**)&p, D3DLOCK_DISCARD));
	int vbIndex = 0;

	// For each living particle.
	for(UINT i = 0; i < m_vAliveParticles.size(); ++i)
	{
		// Copy particle to VB
		p[vbIndex] = *m_vAliveParticles[i];
		++vbIndex;
	}
	HR(m_pVertexBuffer->Unlock());

	// Render however many particles we copied over.
	if(vbIndex > 0)
	{
		HR(m_pD3dDevice->DrawPrimitive(D3DPT_POINTLIST, 0, vbIndex));
	}

	HR(m_pEffect->EndPass());
	HR(m_pEffect->End());

	if (m_pExplosion->GetState())
	{
		m_pExplosion->Render();
	}
}