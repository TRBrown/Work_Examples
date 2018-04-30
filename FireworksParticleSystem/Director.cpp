#include "DXUT.h" 
#include "Director.h"

Director::Director()
{
	m_ctStartTime = 0;
	m_dLastEvent = 0;
}

Director* Director::Instance()
{
	static Director instance;
	return &instance;

}

Director::~Director()
{
	std::list<Fountain*>::iterator i;

	for(i = m_lFountainEvents.begin(); i != m_lFountainEvents.end(); ++i)
	{		
		delete *i;
	}

	std::list<Rocket*>::iterator x;

	for(x = m_lRocketEvents.begin(); x != m_lRocketEvents.end(); ++x)
	{		
		delete *x;
	}
}

void Director::Initialise(IDirect3DDevice9* pd3dDevice)
{
	m_pD3dDevice = pd3dDevice;
}

void Director::UpdateClockCurrent()
{
	//multiply processor ticks by 1000 for ms. Divide by ticks per sec to get true ms
	double diffticks = clock() - m_ctStartTime;
	m_dCurrenTime = (diffticks * 1000) / CLOCKS_PER_SEC; 
} 

void Director::UpdateXMLEvent()
{
	std::vector<XMLEvent>::iterator i;

	for(i = m_vXMLEvent.begin(); i != m_vXMLEvent.end(); ++i)
	{		

		if((m_dCurrenTime > i->iStart) && !i->bActive)
		{
			i->bActive = true;

			switch (i->iEventType)
			{
			case XMLFountain:
				{

					Fountain* newEvent = new Fountain(  m_pD3dDevice,
						*i,
						"Effects\\Particle.fx",
						"Fountain",
						"Textures\\particle.png",
						D3DXVECTOR3(-3.0f, -9.8f, 0.0f),
						2000,
						0.003f);

					m_lFountainEvents.push_back(newEvent);
					break;
				}

			case XMLRocket:
				{
					Rocket* newEvent = new Rocket(  m_pD3dDevice,
						*i,
						"Effects\\Particle.fx",
						"Fountain",
						"Textures\\particle.png",
						D3DXVECTOR3(-3.0f, -9.8f, 0.0f),
						2000,
						0.003f);

					m_lRocketEvents.push_back(newEvent);
					break;
				}
			}

		}
	}


} 

void Director::UpdateParticleSystems(float fElapsedTime)
{	
	std::list<Fountain*>::iterator i;

	for(i = m_lFountainEvents.begin(); i != m_lFountainEvents.end(); i++)
	{		
		Fountain* x = *i;
		
		if ((m_dCurrenTime > (x->GetStart() + x->GetEnd())) && x->GetState())
		{
			x->Deactivate();
		}
		else if(x->m_bFinished)
		{
			delete x;
			i = m_lFountainEvents.erase(i);
		}
		else x->Update(fElapsedTime);

		if (i == m_lFountainEvents.end()) break;
	};

	std::list<Rocket*>::iterator y;

	for(y = m_lRocketEvents.begin(); y != m_lRocketEvents.end(); y++)
	{		
		Rocket* x = *y;

		if ((m_dCurrenTime > (x->GetStart() + x->GetEnd())) && x->GetState())
		{
			x->Deactivate();
		}
		else if(x->m_bFinished)
		{
			delete x;
			y = m_lRocketEvents.erase(y);
		}
		else x->Update(fElapsedTime);

		if (y == m_lRocketEvents.end()) break;
	};


}

void Director::load(const char* pFilename)
{
	TiXmlDocument doc(pFilename);
	if (!doc.LoadFile()) throw 1;

	TiXmlHandle hDoc(&doc);
	TiXmlElement* pElem;
	TiXmlHandle hRoot(0);
	TiXmlElement* pTemp;

	try
	{
		pElem=hDoc.FirstChildElement().Element();
		// should always have a valid root but handle gracefully if it does
		if (!pElem) throw 2;

		// save this for later
		hRoot=TiXmlHandle(pElem);

		//
		//
		//

		pElem=hRoot.FirstChild( "Firework" ).Element();
		for( pElem; pElem; pElem=pElem->NextSiblingElement())
		{

			const std::string type = pElem->Attribute("type");

			if (type == "Fountain")
			{
				XMLEvent Event;				
				
				//Get beginning int
				//
				pElem->QueryIntAttribute("begin", &Event.iStart);

				//Simple Red green blue alpha would of been nice ...
				std::string stringtemp = "colour";
				pElem->QueryValueAttribute(stringtemp, &stringtemp);
				const char* str1 = stringtemp.c_str();
				sscanf_s(str1, "%x", &Event.d3cColour);
				//Add a alpha to it
				DWORD alphahex = 0xFF000000;
				Event.d3cColour += alphahex;

				//Get duration int
				//
				pElem->QueryIntAttribute("duration", &Event.iDuration);
				
				//update the last event time to record the last event
				if(m_dLastEvent < Event.iStart)m_dLastEvent = Event.iStart; 

				//Get Position element and store position info
				// Casting into to float QueryIntAttribute returns int
				pTemp = pElem->FirstChildElement( "Position" );
				int xtemp;
				int ytemp;

				pTemp->QueryIntAttribute("x", &xtemp);
				pTemp->QueryIntAttribute("y", &ytemp);


				Event.v3Position = D3DXVECTOR3(	(float)xtemp,
												(float)ytemp,
												0.0f);

				Event.bActive	= false;
				Event.iEventType = 0;	

		        m_vXMLEvent.push_back(Event);

		
			}

			if (type == "Rocket")
			{
				
				XMLEvent Event;
				
				//Get beginning int
				//
				pElem->QueryIntAttribute("begin", &Event.iStart);

				//Simple Red green blue alpha would of been nice ...
				std::string stringtemp = "colour";
				pElem->QueryValueAttribute(stringtemp, &stringtemp);
				const char* str1 = stringtemp.c_str();
				sscanf_s(str1, "%x", &Event.d3cColour);
				//Add a alpha to it
				DWORD alphahex = 0xFF000000;
				Event.d3cColour += alphahex;

				//Get duration int
				//
				pElem->QueryIntAttribute("duration", &Event.iDuration);
				//update the last even time
				if(m_dLastEvent < Event.iStart)m_dLastEvent = Event.iStart; 

				//Get Position element and store position info
				// Casting into to float QueryIntAttribute returns int
				pTemp = pElem->FirstChildElement( "Position" );
				int pxtemp;
				int pytemp;

				pTemp->QueryIntAttribute("x", &pxtemp);
				pTemp->QueryIntAttribute("y", &pytemp);


				Event.v3Position = D3DXVECTOR3((float)pxtemp,
												   (float)pytemp,
												   0.0f);


				//Get Velocity element and store position info
				// Casting into to float QueryIntAttribute returns int
				pTemp = pElem->FirstChildElement( "Velocity" );
				int vxtemp;
				int vytemp;

				pTemp->QueryIntAttribute("x", &vxtemp);
				pTemp->QueryIntAttribute("y", &vytemp);

				Event.v3Velocity = D3DXVECTOR3((float)vxtemp,
												   (float)vytemp,
												   0.0f);

				
				Event.bActive	= false;
				Event.iEventType = 1;

				m_vXMLEvent.push_back(Event);


			}

		}

		//Give the last firework time to finish
		m_dLastEvent += 2000; 

	}
	catch (int e)
	{
		if (e == 1) MessageBox(0, L"Invalid filename XML", 0, 0); // Invalid filename
		if (e == 2) MessageBox(0, L"Invalid root element XML", 0, 0); // Invalid root

	}
}


void Director::UpdateScene(float fElapsedTime)
{
	
	if(m_dCurrenTime > m_dLastEvent)
	{
		m_ctStartTime = clock();
		m_dCurrenTime = 0;

		std::vector<XMLEvent>::iterator i;

		for(i = m_vXMLEvent.begin(); i != m_vXMLEvent.end(); ++i)
		{
			i->bActive = false;
		}
	}

	UpdateClockCurrent();
	
	UpdateXMLEvent();

	UpdateParticleSystems(fElapsedTime);

}

void Director::RenderScene()
{	
	if(!m_lFountainEvents.empty())
	{
		std::list<Fountain*>::iterator i;

		for(i = m_lFountainEvents.begin(); i != m_lFountainEvents.end(); ++i)
		{		
			ParticleSystem* x = *i;

			x->Render();


		};
	}

	if(!m_lRocketEvents.empty())
	{
		std::list<Rocket*>::iterator y;

		for(y = m_lRocketEvents.begin(); y != m_lRocketEvents.end(); ++y)
		{		
			ParticleSystem* x = *y;

			x->Render();

		}
	}


}