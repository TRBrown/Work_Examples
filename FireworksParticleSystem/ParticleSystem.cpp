#include "DXUT.h" 
#include "Particlesystem.h"
#include "CameraManager.h"
#include "d3dStructures.h"
#include <cassert>

ParticleSystem::ParticleSystem(	IDirect3DDevice9*	pD3dDevice,
								const std::string&	FXFileName, 
								const std::string&	TechniqueName, 
								const std::string&	TextureName, 
								const D3DXVECTOR3&	acceleration, 
								int					maxNumParticles,
								float				timePerParticle):

								m_v3Accel(acceleration), m_fTime(0.0f),
								m_iMaxNumParticles(maxNumParticles), m_iTimePerParticle(timePerParticle)
{
	m_pD3dDevice = pD3dDevice;
	m_bActive = true;
	m_bFinished = false;

	// Allocate memory for maximum number of particles.
	m_vParticles.resize(m_iMaxNumParticles);
	m_vAliveParticles.reserve(m_iMaxNumParticles);
	m_vDeadParticles.reserve(m_iMaxNumParticles);

	// They start off all dead.
	for(int i = 0; i < m_iMaxNumParticles; ++i)
	{
		m_vParticles[i].lifeTime = -1.0f;
		m_vParticles[i].initialTime = 0.0f;
	}

// 	// Allocate memory for maximum number of particles.
// 	m_vAliveParticles.reserve(m_iMaxNumParticles);
// 	m_vDeadParticles.reserve(m_iMaxNumParticles);
// 
// 	// They start off all dead.
// 	for(int i = 0; i < m_iMaxNumParticles; ++i)
// 	{
// 		Particle* newParticle = new Particle();
// 		newParticle->m_fLifeTime = -1.0f;
// 		newParticle->m_fInitialTime = 0.0f;
// 		m_vParticles.push_back(newParticle);
// 	}

	wchar_t* textureChar = 0;
	wchar_t* EffectChar = 0;
	widen(textureChar, TextureName);
	widen(EffectChar, FXFileName);

	// Create the texture.
	HR(D3DXCreateTextureFromFile(pD3dDevice, textureChar, &m_pTexture));

	// Create the FX.
	ID3DXBuffer* errors = 0;
	HR(D3DXCreateEffectFromFile(pD3dDevice, EffectChar,
		0, 0, D3DXSHADER_DEBUG, 0, &m_pEffect, &errors));

	if( errors )
		MessageBoxA(0, (char*)errors->GetBufferPointer(), 0, 0);

	m_hTech    = m_pEffect->GetTechniqueByName(TechniqueName.c_str());
	m_hWVP     = m_pEffect->GetParameterByName(0, "gWVP");
	m_hEyePosL = m_pEffect->GetParameterByName(0, "gEyePosL");
	m_hTex     = m_pEffect->GetParameterByName(0, "gTex");
	m_hTime    = m_pEffect->GetParameterByName(0, "gTime");
	m_hAccel   = m_pEffect->GetParameterByName(0, "gAccel");
	m_hViewportHeight = m_pEffect->GetParameterByName(0, "gViewportHeight");

	// We don't need to set these every frame since they do not change.
	HR(m_pEffect->SetTechnique(m_hTech));
	HR(m_pEffect->SetValue(m_hAccel, m_v3Accel, sizeof(D3DXVECTOR3)));
	HR(m_pEffect->SetTexture(m_hTex, m_pTexture));

	HR(pD3dDevice->CreateVertexBuffer(m_iMaxNumParticles*sizeof(VertexParticle),
		D3DUSAGE_DYNAMIC|D3DUSAGE_WRITEONLY|D3DUSAGE_POINTS,
		0, D3DPOOL_DEFAULT, &m_pVertexBuffer, 0));

	delete [] textureChar;
	delete [] EffectChar;
}

ParticleSystem::~ParticleSystem()
{
// 	std::vector<Particle*>::iterator i;
// 
// 	for(i = m_vParticles.begin(); i != m_vParticles.end(); ++i)
// 	{		
// 		delete *i;
// 	}
	
	m_pVertexBuffer->Release();
	m_pVertexBuffer = 0;
	m_pTexture->Release();
	m_pTexture = 0;
	m_pEffect->Release();
	m_pEffect = 0;
}

float ParticleSystem::getTime()
{
	return m_iTimePerParticle;
}

void ParticleSystem::setTime(float t)
{
	m_iTimePerParticle = t;
}

void ParticleSystem::setWorldMatrix(const D3DXMATRIX& world)
{
	m_mWorld = world;

	// Compute the change of coordinates matrix that changes coordinates 
	// relative to world space so that they are relative to the particle
	// system's local space.
	D3DXMatrixInverse(&m_mInvWorld, 0, &m_mWorld);
}

void ParticleSystem::addParticle()
{
	if( m_vDeadParticles.size() > 0)
	{
		// Reinitialize a particle.
		VertexParticle* p = m_vDeadParticles.back();
		initParticle(*p);

		// No longer dead.
		m_vDeadParticles.pop_back();
		m_vAliveParticles.push_back(p);
	}
}

void ParticleSystem::onLostDevice()
{
	HR(m_pEffect->OnLostDevice());

	// Default pool resources need to be freed before reset.
	m_pVertexBuffer->Release();
	m_pVertexBuffer = 0;
}

void ParticleSystem::onResetDevice()
{
	HR(m_pEffect->OnResetDevice());

	// Default pool resources need to be recreated after reset.
	if(m_pVertexBuffer == 0)
	{
		HR(m_pD3dDevice->CreateVertexBuffer(m_iMaxNumParticles*sizeof(VertexParticle),
			D3DUSAGE_DYNAMIC|D3DUSAGE_WRITEONLY|D3DUSAGE_POINTS,
			0, D3DPOOL_DEFAULT, &m_pVertexBuffer, 0));
	}
}


void ParticleSystem::Update(float dt)
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
	if (!m_bActive && m_vAliveParticles.empty()) 
	{
		m_bFinished = true;
	}
}

/*
void ParticleSystem::Update(float dt)
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
		if( (m_fTime - m_vParticles[i]->m_fInitialTime) > m_vParticles[i]->m_fInitialTime)
		{
			m_vDeadParticles.push_back(m_vParticles[i]);
		}
		else
		{
			m_vAliveParticles.push_back(m_vParticles[i]);
		}    
	}

	if (!m_bActive && m_vAliveParticles.empty()) 
	{
		m_bFinished = true;
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
}*/

void ParticleSystem::Render()
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
}