#include "BhMaster.hpp"

BhMaster::BhMaster()
{
}
BhMaster::~BhMaster()
{
}
int BhMaster::Init()
{
	return 0;
}
void BhMaster::Clear()
{
}
int BhMaster::AddModule(unsigned uModule, IBhModule* pModule)
{
    std::map<unsigned, IBhModule*>::iterator iter = m_moduleMap.find(uModule);
    if (iter != m_moduleMap.end())
    {
        return -1;
    }
    m_moduleMap[uModule] = pModule;
    return 0;
}
int BhMaster::Start()
{
	m_bWantRun = true;
	return this->IBhThread::Start();
}
void BhMaster::Stop()
{
    m_bWantRun = false;
}
void BhMaster::Run()
{
    std::map<unsigned, IBhModule*>::iterator itModule;
    std::map<unsigned, IBhModule*>::iterator itRecvModule;
	std::list<IBhMsg*> msgList;
	std::list<IBhMsg*>::iterator itMsg;
    for (itModule = m_moduleMap.begin(); itModule != m_moduleMap.end(); ++itModule)
    {
        itModule->second->Start();
    }
    while (m_bWantRun)
    {
        for (itModule = m_moduleMap.begin(); itModule != m_moduleMap.end() && m_bWantRun; ++itModule)
        {
			itModule->second->GetSendMessageList(msgList);
            for (itMsg = msgList.begin(); itMsg != msgList.end() && m_bWantRun; )
            {
				if (*itMsg)
				{
					itRecvModule = m_moduleMap.find((*itMsg)->GetRecvModule());
					if (itRecvModule == m_moduleMap.end())
					{//can not find msg recv module;
						delete *itMsg;
					}
					else
					{
						itRecvModule->second->OnRecvMessage(*itMsg);
					}
				}
				itMsg = msgList.erase(itMsg);
            }
        }
    }
    for (itModule = m_moduleMap.begin(); itModule != m_moduleMap.end(); ++itModule)
    {
        itModule->second->Stop();
    }
    for (itModule = m_moduleMap.begin(); itModule != m_moduleMap.end(); ++itModule)
    {
        itModule->second->Join();
    }
}
