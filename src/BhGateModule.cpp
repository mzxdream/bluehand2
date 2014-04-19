#include "BhGateModule.hpp"

BhGateModule::BhGateModule(unsigned uMaxWorks, unsigned uMaxListen, const std::string& strIP, int nPort)
	:m_uMaxWorks(uMaxWorks)
	,m_pWorkArray(NULL)
	,m_listenSock(uMaxListen, strIP, nPort)
{
}
BhGateModule::~BhGateModule()
{
	m_uMaxWorks = 0;
	m_listenSock.Clear();
	for (int i = 0; i < m_uMaxWorks; ++i)
	{
		m_pWorkArray[i].Clear();
	}
	delete[] m_pWorkArray;
	m_pWorkArray = NULL;
	this->IBhActiveModule::Clear();
}
int BhGateModule::Init()
{
	if (this->IBhActiveModule::Init() != 0)
	{
		return -1;
	}
	if (m_listenSock.Init() != 0)
	{
		return -2;
	}
	m_pWorkArray = new BhGateWork[m_uMaxWorks]((m_listenSocket.MaxListen() + 1)/2)
	if (!m_pWorkArray)
	{
		return -3;
	}
	for (int i=0; i < m_uMaxWorks; ++i)
	{
		m_pWorkArray[i].ListenSocket(&m_listenSock);
		m_pWorkArray[i].ListenMutex(&m_listenMutex);
		if (m_pWorkArray[i].Init() != 0)
		{
			return -4;
		}
	}

	return 0;
}
void BhGateModule::Clear()
{
	m_uMaxWorks = 0;
	m_listenSock.Clear();
	for (int i = 0; i < m_uMaxWorks; ++i)
	{
		m_pWorkArray[i].Clear();
	}
	delete[] m_pWorkArray;
	m_pWorkArray = NULL;
	this->IBhActiveModule::Clear();
}
void BhGateModule::Run()
{
	int i = 0;
	for (i = 0; i < m_uMaxWorks; ++i)
	{
		m_pWorkArray[i].Start();
	}
	while (m_bWantRun)
	{
		ProcessMessage();
	}
	for (i = 0; i < m_uMaxWorks; ++i)
	{
		m_pWorkArray[i].Stop();
	}
	for (i = 0; i < m_uMaxWorks; ++i)
	{
		m_pWorkArray[i].Join();
	}
}
void BhGateModule::ProcessRemoteMessage()
{
	std::list<IBhMsg*> msgList;
	GetRecvMessageList(msgList);
	std::list<IBhMsg*>::iterator iter = msgList.begin();
	for (; iter != msgList.end(); ++iter)
	{
		//handle msg
		//clear
	}
}
void BhGateModule::ProcessWorkMessage()
{
	std::list<IBhMsg*> msgList;
	for (int i = 0; i < m_uMaxWorks; ++i)
	{
		GetRecvMessageList(msgList);
		//handle msg
		//clear list
	}
}
void BhGateModule::ProcessMessage()
{
	ProcessRemoteMessage();
	ProcessWorkMessage();
}
