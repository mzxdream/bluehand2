#include "BhGateWork.hpp"

BhGateWork::BhGateWork(unsigned uMaxEvents)
	:m_epoll(uMaxEvents)
	,m_pListenSock(NULL)
	,m_pListenMutex(NULL)
	,m_uListenCount(0)
	,m_nAcceptDisabled(0)
{
}
BhGateWork::~BhGateWork()
{
}
int BhGateWork::Init()
{
	if (this->IBhActiveModule::Init() != 0)
	{
		return -1;
	}
	if (m_epoll.Init() != 0)
	{
		return -2;
	}
	m_uListenCount = 0;
	m_pListenSock = NULL;
	m_pListenMutex = NULL;
	m_nAcceptDisabled = 0;
	m_timer = 0;
	m_epoll.ReadSignal() = MakeDelegate(this, &BhGateWork::OnSockRead);
	m_epoll.WriteSignal() = MakeDelegate(this, &BhGateWork::OnSockWrite);
	m_epoll.ErrorSignal() = MakeDelegate(this, &BhGateWork::OnSockError);
	m_epoll.CloseSignal() = MakeDelegate(this, &BhGateWork::OnSockClose);
	return 0;
}
void BhGateWork::Clear()
{
	m_epoll.Clear();
	m_pListenSock = NULL;
	m_pListenMutex = NULL;
	m_uListenCount = 0;
	m_nAcceptDisabled = 0;
	m_timer = 0;
	this->IBhActiveModule::Clear();
}
void BhGateWork::ProcessEvents()
{
	int nTimer = GetEventTimer();
	bool bListen = false;
	if (-1 == nTimer || nTimer > 500)
	{
		nTimer = 500;
	}
	if (m_nAcceptDisabled > 0)
	{
		m_nAcceptDisabled--;
	}
	else
	{
		if (m_pListenMutex->TryLock() == 0)
		{
			bListen = true;
			m_epoll.AddConnection(m_pListenSock);
		}
	}
	m_epoll.WaitEvents(nTimer);
	if (bListen)
	{
		ProcessListenEvents();
		m_pListenMutex->UnLock();
	}
	ProcessSocketEvents();
	m_timer += nTimer;
	ProcessTimerEvents();
	ProcessRemoteMessage();
}
void BhGateWork::Run()
{
	if (!m_pListenSock || !m_pListenMutex)
	{
		return;
	}
	while (WantRun())
	{
		ProcessEvents();
	}
}
void BhGateWork::ListenSocket(BhListenSocket* pSock)
{
	m_pListenSock = pSock;
}
void BhGateWork::ListenMutex(BhListenMutex* pMutex)
{
	m_pListenMutex = pMutex;
}
void* BhGateWork::OnSockRead(void* pParam)
{
}
void* BhGateWork::OnSockWrite(void* pParam)
{
}
void* BhGateWork::OnSockError(void* pParam)
{
}
void* BhGateWork::OnSockClose(void* pParam)
{
}
