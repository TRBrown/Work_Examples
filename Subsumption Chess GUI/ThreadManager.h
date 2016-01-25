#ifndef THREADMANAGER_H
#define THREADMANAGER_H

#pragma warning(disable:4995)

#include "d3dx9.h" 
#include "d3d9.h"
#include "D3D9.h"
#include <list>
#include "BoardManager.h"
#include "AIThread.h"
#include <vector>
#include "Custom_Services.h"


class ThreadManager
{
private:
	
	//Thread Related globals 
	AIThread* m_pActiveThreadObject;

	BoardManager* m_pBoardManager; // pointer to board manager

	ThreadManager();

	ThreadManager(const ThreadManager &);

public:


	~ThreadManager();

//	void			AddLayer(Layer* val){m_lpLayers.push_back(val);}

	void	CreateAIThreadObject(int Movenum);
	void	RunAIThread();

	static ThreadManager* Instance(Custom_Services* GameManagers);
};

#endif

