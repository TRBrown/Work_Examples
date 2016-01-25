#include "DXUT.h"
#include "AIThread.h"
#include "AI_Stage0.h"

AIThread::AIThread() 
{
	m_pAI = new AI_Stage0();	
}

AIThread::~AIThread()
{
	delete m_pAI;
}

HANDLE AIThread::StartThread(AIThread* parg)
{
	//Arg(arg); // store user data ** not used
	hThread = CreateThread( 
		NULL,                   // default security attributes
		0,                      // use default stack size  
		EntryPoint,       // thread function name
		parg,			// argument to thread function 
		0,                      // use default creation flags 
		&dwThreadId);   // returns the thread identifier 
	return hThread;



}

void AIThread::Run()
{
	Setup();
	Execute( Arg() );

}

/*static */
DWORD WINAPI AIThread::EntryPoint(LPVOID lpParam)
{
	AIThread* tmp = (AIThread*)lpParam;
	tmp->Run();
	
	return 0;
}

void AIThread::Setup()
{
	// Any thread setup code here code (Post construction | Pre-execution)
}

void AIThread::Execute(void* arg)
{
	
		m_pAI->Execute(m_pBoardManager, m_iMovNumb);


	m_pBoardManager->IncrementMoveNumber(); //Increment Move number
	m_pBoardManager->SignalAIFinished(dwThreadId); // Mark thread as done all results processed
}

AIThread* AIThread::Instance(BoardManager* pBoardManager, int MovNumb)
{
	static AIThread instance;
	instance.m_pBoardManager = pBoardManager;
	instance.m_iMovNumb = MovNumb;

	return &instance;

}
