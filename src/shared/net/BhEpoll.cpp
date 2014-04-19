#include "BhEpoll.hpp"

BhEpoll::BhEpoll(int nMaxEvents)
    :m_nEpoll(-1)
    ,m_pEventList(NULL)
    ,m_nMaxEvents(nMaxEvents)
{
}
BhEpoll::~BhEpoll()
{
    Close();
}
bool BhEpoll::Init()
{
    if (-1 == (m_nEpoll = ::epoll_create1(0)))
    {
        return false;
    }
    m_pEventList = new struct epoll_event[m_nMaxEvents];
    if (!m_pEventList)
    {
        ::close(m_nEpoll);
        m_nEpoll = -1;
        return false;
    }
    return true;
}
void BhEpoll::Clear()
{
    if (-1 != m_nEpoll)
    {
        ::close(m_nEpoll);
		m_nEpoll = -1;
    }
    if (m_pEventList)
    {
        delete[] m_pEventList;
		m_pEventList = NULL;
    }
}
bool BhEpoll::AddEvent(BhTcpSocket* pSock, unsigned uEvents)
{
    struct epoll_event ee;
    int op;
    ee.data.ptr = static_cast<void *>(pSock);
    if (uEvents & BHEPOLL_IN)
    {
        ee.events = EPOLLIN | EPOLLRDHUP;
        if (pSock->WriteActive())
        {
            ee.events |= BHEPOLL_OUT;
            op = EPOLL_CTL_MOD;
        }
        else
        {
            op = EPOLL_CTL_ADD;
        }
    }
    else
    {
        ee.events = EPOLLOUT;
        if (pSock->ReadActive())
        {
            ee.events |= (EPOLLIN | EPOLLRDHUP);
            op = EPOLL_CTL_MOD;
        }
        else
        {
            op = EPOLL_CTL_ADD;
        }
    }
    if (uEvents & BHEPOLL_ET)
    {
        ee.events |= EPOLLET;
    }
    if (::epoll_ctl(m_nEpoll, op, pSock->GetSocket(), &ee) == -1)
	{
		return false;
    }
    if (BHEPOLL_IN & uEvents)
    {
        pSock->ReadActive(true);
    }
    else
    {
        pSock->WriteActive(true);
    }
    return true;
}
bool BhEpoll::DelEvent(BhTcpSocket* pSock, unsigned uEvents)
{
    int op;
    struct epoll_event ee;
    if (BHEPOLL_IN & uEvents)
    {
        if (pSock->WriteActive())
        {
            op = EPOLL_CTL_MOD;
            ee.events = EPOLLOUT;
            ee.data.ptr = static_cast<void*>(pSock);
            if (uEvents & BHEPOLL_ET)
            {
                ee.events |= EPOLLET;
            }
        }
        else
        {
            op = EPOLL_CTL_DEL;
            ee.events = 0;
            ee.data.ptr = NULL;
        }
    }
    else
    {
        if (pSock->ReadActive())
        {
            op = EPOLL_CTL_MOD;
            ee.events = (EPOLLIN | EPOLLRDHUP);
            ee.data.ptr = static_cast<void*>(pSock);
        }
        else
        {
            op = EPOLL_CTL_DEL;
            ee.events = 0;
            ee.data.ptr = NULL;
        }
    }
    if (::epoll_ctl(m_nEpoll, op, pSock->GetSocket(), &ee) == -1)
    {
        return false;
    }
    if (BHEPOLL_IN & uEvents)
    {
        pSock->ReadActive(false);
    }
    else
    {
        pSock->WriteActive(false);
    }
    return true;
}
bool BhEpoll::AddConnection(BhListenSocket* pSock)
{
    struct epoll_event ee;
    ee.events = EPOLLIN | EPOLLOUT | EPOLLET | EPOLLRDHUP;
    ee.data.ptr = static_cast<void*>(pSock);
    if (::epoll_ctl(m_nEpoll, EPOLL_CTL_ADD, pSock->GetSocket(), &ee) == -1)
    {
        return false;
    }
    pSock->ReadActive(true);
    return true;
}
bool BhEpoll::DelConnection(BhListenSocket *pSock)
{
    if (::epoll_ctl(m_nEpoll, EPOLL_CTL_DEL, pSock->GetSocket(), &ee) == -1)
	{
        return false;
    }
    pSock->ReadActive(false);
    return true;
}
int BhEpoll::WaitEvents(int nTimeOut)
{
    int nEvents = ::epoll_wait(m_nEpoll, m_pEventList, m_nMaxEvents, nTimeOut);
    for (int i = 0; i < nEvents; ++i)
	{
        struct epoll_event& e = m_pEventList[i];
        BhSocket* pSock = static_cast<BhSocket*>(e.data.ptr);
        if ((e.events & EPOLLERR) || ((e.events & EPOLLHUP) && !(e.events & EPOLLIN)))
        {
            m_sigError(pSock);
        }
        else if (e.events & EPOLLIN)
        {
            if (e.events & EPOLLRDHUP)
            {
                m_sigClose(pSock);
            }
            else
            {
                m_sigRead(pSock);
            }
        }
        else if (e.events & EPOLLOUT)
        {
            m_sigWrite(pSock);
        }
    }
    return 0;
}
BhSignal& BhEpoll::ReadSignal()
{
    return m_sigRead;
}
BhSignal& BhEpoll::WriteSignal()
{
    return m_sigWrite;
}
BhSignal& BhEpoll::ErrorSignal()
{
    return m_sigError;
}
BhSignal& BhEpoll::CloseSignal()
{
    return m_sigClose;
}
