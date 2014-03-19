#include "BhEpoll.hpp"

BhEpoll::BhEpoll()
    :m_nEpoll(-1)
    ,m_pEventList(NULL)
    ,m_nMaxEvents(0)
{
}
BhEpoll::~BhEpoll()
{
    Clear();
}
bool BhEpoll::Init(int nMaxEvents)
{
    if (-1 == (m_nEpoll = ::epoll_create1(0)))
    {
        return false;
    }
    m_nMaxEvents = nMaxEvents;
    m_pEventList = new struct epoll_event[nMaxEvents];
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
    }
    if (m_pEventList)
    {
        delete[] m_pEventList;
    }
}
bool BhEpoll::AddEvent(BhTcpSocket *pSock, BhEpollEvent event, BhEpollFlag flag)
{
    int op;
    uint32_t events;
    struct epoll_event ee;
    if (BhEpollEvent::Read == event)
	{
		events = EPOLLIN | EPOLLRDHUP;
		if (pSock->WriteActive())
        {
            events |= EPOLLOUT;
            op = EPOLL_CTL_MOD;
        }
        else
        {
            op = EPOLL_CTL_ADD;
        }
    }
    else
    {
        events = EPOLLOUT;
        if (pSock->ReadActive())
        {
            events |= (EPOLLIN | EPOLLRDHUP);
            op = EPOLL_CTL_MOD;
        }
        else
        {
            op = EPOLL_CTL_ADD;
        }
    }
    if (BhEpollFlag::Edge == flag)
	{
		events |= EPOLLET;
	}
    ee.events = events;
    ee.data.ptr = static_cast<void *>(pSock);
    if (::epoll_ctl(m_nEpoll, op, pSock->GetSocket(), &ee) == -1)
	{
		return false;
	}
    if (BhEpollEvent::Read == event)
	{
		pSock->ReadActive(true);
	}
    else
	{
		pSock->WriteActive(true);
	}
    return true;
}
bool BhEpoll::DelEvent(BhTcpSocket *pSock, BhEpollEvent event, BhEopllFlag flag)
{
    int op;
    struct epoll_event ee;
    if (BhEpollEvent::Read == event)
	{
		if (pSock->WriteActive())
        {
            op = EPOLL_CTL_MOD;
            ee.events = EPOLLOUT;
            ee.data.ptr = static_cast<void*>(pSock);
            if (BhEpollFlag::Edge == flag)
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
    if (BhEpollEvent::Read == event)
	{
		pSock->ReadActive(false);
	}
    else
	{
		pSock->WriteActive(false);
	}
    return true;
}
bool BhEpoll::AddConnection(BhListenSocket *pSock)
{
    struct epoll_event ee;
    ee.events = EPOLLIN | EPOLLOUT | EPOLLET | EPOLLRDHUP;
    ee.data.ptr = static_cast<void*>(pSock);
    if (::epoll_ctl(m_nEpoll, EPOLL_CTL_ADD, pSock->GetSocket(), &ee) == -1)
    {
        return false;
    }
    pSock->ReadActive(true);
    pSock->WriteActive(true);
    return true;
}
bool BhEpoll::DelConnection(BhListenSocket *pSock)
{
    if (::epoll_ctl(m_nEpoll, EPOLL_CTL_DEL, pSock->GetSocket(), &ee) == -1)
	{
        return false;
    }
    pSock->ReadActive(false);
    pSock->WriteActive(false);
    return true;
}
bool BhEpoll::WaitEvents(int nTimeOut)
{
    int nEvents = ::epoll_wait(m_nEpoll, m_pEventList, m_nMaxEvents, nTimeOut);
    for (int i = 0; i < nEvents; ++i)
	{
        struct epoll_event &e = m_pEventList[i];
        IBhSocket *pSock = static_cast<IBhSocket*>(e.data.ptr);
        if ((e.events & EPOLLERR) || ((e.events & EPOLLHUP) && !(e.events & EPOLLIN)))
        {
            pSock->Error(true);
        }
        else if (e.events & EPOLLIN)
        {
            if (e.events & EPOLLRDHUP)
            {
                pSock->ReadClosed(true);
            }
            else
            {
                pSock->ReadReady(true);
            }
        }
        else if (e.events & EPOLLOUT)
        {
            pSock->WriteReady(true);
        }
    }
    return true;
}
