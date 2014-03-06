#ifndef _BHMASTER_HPP_
#define _BHMASTER_HPP_

#include <map>
#include "./shared/thread/IBhThread.hpp"
#include "./msg/IBhMsg.hpp"
#include "./module/IBhModule.hpp"

class BhMaster
    :public IBhThread
{
public:
    BhMaster();
    virtual ~BhMaster();
public:
    bool AddModule(unsigned uModule, IBhModule* pModule);
    void Stop();
protected:
    virtual void Run();
protected:
    std::map<unsigned, IBhModule*> m_moduleMap;
    volatile bool m_bWantRun;
};

#endif
