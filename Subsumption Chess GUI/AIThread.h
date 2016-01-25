#ifndef AITHREAD_H
#define AITHREAD_H

#pragma warning(disable:4995)

#include "d3dx9.h" 
#include "d3d9.h"
#include "D3D9.h"

#include <list>
#include "BoardManager.h"
#include "Layer.h"
#include <vector>

class AIThread
{
private:

	DWORD   dwThreadId;
	HANDLE  hThread;

	int m_iMovNumb;

	BoardManager* m_pBoardManager; // pointer to board manager
	Layer* m_pAI; // List of stage 0 layers

	void * Arg_;//Unused currently

	//Singleton constructor
	AIThread();

	AIThread(const AIThread &);

protected:
	
	
	static DWORD WINAPI EntryPoint(void*); // Entry point from CreateThread
	
	virtual void Setup(); // For setup stuff after thread started
	
	virtual void Execute(void*); //Start the execute
	
	//Accessors to a argument -  Not used currently
	void * Arg() const {return Arg_;}
	
	void Arg(void* a){Arg_ = a;} // Not needed currently

public:

	~AIThread();

	void Run(); // Do not touch

	static AIThread* Instance(BoardManager* pBoardManager, int MovNumb);
	
	DWORD GetThreadID()const{return dwThreadId;}

	HANDLE StartThread(AIThread* parg);
};

#endif