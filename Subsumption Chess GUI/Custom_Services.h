#ifndef CUSTOM_SERVICES_H
#define CUSTOM_SERVICES_H
#pragma warning(disable:4995)

#include <vector>


class Custom_Services
{
private:
	
	std::vector<void*> m_vServices;
	std::vector<char*> m_vDefinitions;

	Custom_Services();

	Custom_Services(const Custom_Services &);

public:

	static Custom_Services* Instance();

	~Custom_Services();

	int CharLength(const char* phrase);

	template <class T>
	void AddService(T* service, char* phrase)
	{
		//const char* classname = typeid( service ).name();
		//char* tempchar = new char [CharLength(classname)];
		//while( *(tempchar++) = *(classname++) );
		m_vDefinitions.push_back(phrase);
		m_vServices.push_back(service);
	};

	template <class T>
	T* GetService(char* phrase)
	{
		unsigned int i;

		for(i = 0 ; i < m_vDefinitions.size() ; i++)
		{		
			if( !strcmp( phrase, m_vDefinitions[i] ) )
			{
				return static_cast< T* >(m_vServices[i]);
			}

		}

		return NULL;
	};
	
	//template <class T> T* GetService(char* phrase);

	bool DeleteService(char* phrase);

	void DeleteAll();


};

#endif