#include "DXUT.h"
#include "ThreadManager.h"

ThreadManager::ThreadManager()
{

};

ThreadManager* ThreadManager::Instance(Custom_Services* GameManagers)
{
	static ThreadManager instance;
	instance.m_pBoardManager = GameManagers->GetService<BoardManager>("BoardManager");

	return &instance;

}

ThreadManager::~ThreadManager()
{
	
}

void	ThreadManager::CreateAIThreadObject(int MovNumb)
{
	m_pBoardManager->AddAIMoveRow();
	m_pActiveThreadObject = AIThread::Instance(m_pBoardManager, MovNumb);
	m_pActiveThreadObject->StartThread(m_pActiveThreadObject);
	m_pBoardManager->SetActiveThreadIdent(m_pActiveThreadObject->GetThreadID());
}

void	ThreadManager::RunAIThread()
{
	m_pActiveThreadObject->Run();

}

