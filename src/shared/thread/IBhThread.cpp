#include "IBhThread.hpp"

IBhThread::IBhThread()
{
}
IBhThread::~IBhThread()
{
}
int IBhThread::Start()
{
    return ::pthread_create(&m_tid, NULL, ThreadMain, this);
}
int IBhThread::Join()
{
    return ::pthread_join(m_tid, NULL);
}
pthread_t IBhThread::GetThreadID() const
{
    return m_tid;
}
void* IBhThread::ThreadMain(void *pParam)
{
    IBhThread *pThread = static_cast<IBhThread*>(pParam);
    if (pThread)
    {
         pThread->Run();
         pThread->m_tid = 0;
	 return static_cast<void *>(0);
    }
    return static_cast<void *>(-1);
}
