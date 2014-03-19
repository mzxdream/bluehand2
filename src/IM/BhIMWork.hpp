#ifndef _BHIMWORK_HPP_
#define _BHIMWORK_HPP_

#include <net/BhEpoll.hpp>
#include <thread/IBhThread.hpp>

class BhIMWork
{
public:
	BhIMWork();
	virtual ~BhIMWork();
public:
	void Stop(){m_bRun = true;}
	bool Init();
	void Clear();
	virtual void Run()
	{
		while (m_bRun)
		{
			trylocklistensocket();
			m_epoll.WaitEvents();
			acceptsome()
			updatetimes();
			handlesocket();
		}
	}
private:
	volatile bool m_bRun;
	BhEpoll m_epoll;
};

#endif
