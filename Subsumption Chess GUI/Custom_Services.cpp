#include "DXUT.h" 
#include "Custom_Services.h"


Custom_Services::Custom_Services()
{

}


Custom_Services* Custom_Services::Instance()
{
	static Custom_Services instance;
	return &instance;

}


Custom_Services::~Custom_Services()

{
	DeleteAll();
}


int Custom_Services::CharLength(const char* phrase)
{
	int length = -1, i = 0;
	while(length == -1)
	{
		if (phrase[i] == '\0')
		{
			i++;
			length = i;
		}
		i++;
	}
	return length;
}

bool Custom_Services::DeleteService(char* phrase)
{
	unsigned int i;

	for(i = 0 ; i < m_vDefinitions.size(); i++)
	{		
		if( strcmp( phrase, m_vDefinitions[i] ) )
		{
			delete m_vServices[i];
			m_vServices.erase(m_vServices.begin()+i);
			m_vDefinitions.erase(m_vDefinitions.begin()+i);
			return true;
		}

	}

	return false;
}


void Custom_Services::DeleteAll()
{
	unsigned int i;

	for(i = 0 ; i < m_vServices.size(); i++)
	{	
		m_vServices.erase(m_vServices.begin()+i);
		m_vDefinitions.erase(m_vDefinitions.begin()+i);

	}

}