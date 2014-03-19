#include "BhMaster.hpp"

BhMaster::BhMaster()
{
}
BhMaster::~BhMaster()
{
}
bool BhMaster::AddModule(unsigned uModule, IBhModule* pModule)
{
    std::map<unsigned, IBhModule*>::iterator iter = m_moduleMap.find(uModule);
    if (iter != m_moduleMap.end())
    {
        return false;
    }
    m_moduleMap[uModule] = pModule;
    return true;
}
void BhMaster::Stop()
{
    m_bWantRun = false;
}
void BhMaster::Run()
{
    m_bWantRun = true;
    std::map<unsigned, IBhModule*>::iterator itModule;
    std::map<unsigned, IBhModule*>::iterator itRecvModule;
    IBhMsg* pMsg = NULL;
    for (itModule = m_moduleMap.begin(); itModule != m_moduleMap.end(); ++itModule)
    {
        itModule->second->Start();
    }
    while (m_bWantRun)
    {
        for (itModule = m_moduleMap.begin(); itModule != m_moduleMap.end() && m_bWantRun; ++itModule)
        {
            while ((pMsg = itModule->second->GetSendMsg()) && m_bWantRun)
            {
                itRecvModule = m_moduleMap.find(pMsg->GetRecvModule());
                if (itRecvModule == m_moduleMap.end())
                {//can not find msg recv module;
                    delete pMsg;
                }
                else
                {
                    itRecvModule->second->OnMsg(pMsg);
                }
            }
        }
        for (itModule = m_moduleMap.begin(); itModule != m_moduleMap.end() && m_bWantRun; ++itModule)
        {
            itModule->second->OnMsgEnd();
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
