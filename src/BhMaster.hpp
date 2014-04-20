#ifndef _BHMASTER_HPP_
#define _BHMASTER_HPP_

#include <map>
#include <shared/thread/IBhThread.hpp>
#include <shared/module/IBhModule.hpp>
#include <shared/msg/IBhMsg.hpp>

class BhMaster
    :public IBhThread
{
public:
    BhMaster();
    virtual ~BhMaster();
private:
	BhMaster& operator=(const BhMaster&);
	BhMaster(const BhMaster&);
public:
	int Init();
	void Clear();
    int AddModule(unsigned uModule, IBhModule* pModule);
	int Start();
    void Stop();
protected:
    virtual void Run();
protected:
    std::map<unsigned, IBhModule*> m_moduleMap;
    volatile bool m_bWantRun;
};

#endif //_BHMASTER_HPP_
