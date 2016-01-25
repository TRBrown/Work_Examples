#ifndef DIRECTOR_H
#define DIRECTOR_H
#pragma warning(disable:4995)

#include "tinyxml/tinyxml.h"
#include "d3dx9.h" 
#include "d3d9.h"
#include "Particlesystem.h"
#include "Rocket.h"
#include "Fountain.h"
#include "d3dStructures.h"
#include <string>
#include <list>
#include <iostream>
#include <time.h>


class Director
{
private:

	//instance pointer
	Director* m_pDirector;
	IDirect3DDevice9* m_pD3dDevice;

	std::vector<XMLEvent> m_vXMLEvent;

	//XML Loaded ParticleSystems
	std::list<Fountain*> m_lFountainEvents;
	std::list<Rocket*> m_lRocketEvents;

	//timing in milliseconds ms

	clock_t m_ctStartTime;
	double m_dCurrenTime;
	double m_dLastEvent;

	Director();

	Director(const Director &);

protected:


public:

	static Director* Instance();

	~Director();

	void Initialise(IDirect3DDevice9* pd3dDevice);

	void UpdateClockCurrent();

	void UpdateXMLEvent();
	void UpdateParticleSystems(float fElapsedTime);

	void load(const char* pFilename);

	void UpdateScene(float fElapsedTime);

	void RenderScene();


};




#endif