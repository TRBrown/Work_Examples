#ifndef PARTICLE_H
#define PARTICLE_H
#pragma warning(disable:4995)
#pragma once

#include <d3d9.h>
#include <d3dx9tex.h>
#include "d3dStructures.h"

class Particle
{

public:
	

	D3DXVECTOR3 m_v3Position;
	D3DXVECTOR3 m_v3Velocity;
	float       m_fInitialSize; // In pixels.
	float       m_fInitialTime;
	float       m_fLifeTime;
	float       m_fMass;
	D3DCOLOR    m_d3cColour;

	Particle();
	
	VertexParticle getVertexStruct()
	{
		VertexParticle x;
		x.psPosition = m_v3Position;
		x.initialVelocity = m_v3Velocity;
		x.initialSize = m_fInitialSize; // In pixels.
		x.initialTime = m_fInitialTime;
		x.lifeTime = m_fLifeTime;
		x.mass = m_fMass;
		x.colour = m_d3cColour;
	
		return x;
	};

	~Particle();

};

#endif

