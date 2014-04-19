#ifndef _BHGATEMODULE_HPP_
#define _BHGATEMODULE_HPP_

#include "BhGateWork.hpp"

class BhGateModule
	:public IBhActiveModule<IBhMsg>
{
public:
	BhGateModule(unsigned uMaxWorks, unsigned uMaxListen, const std::string& strIP = "", int nPort = 0);
	virtual ~BhGateModule();
public:
	virtual int Init();
	virtual void Clear();
	virtual void Run();
protected:
	void ProcessMessage();
	void ProcessWorkMessage();
	void ProcessRemoteMessage();
private:
	unsigned m_uMaxWorks;
	BhGateWork* m_pWorkArray;
	BhListenSocket m_listenSock;
	BhMutex m_listenMutex;
};

#endif //_BHGATEMODULE_HPP_
