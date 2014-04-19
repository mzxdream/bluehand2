#include "BhSocket.hpp"

BhSocket::BhSocket(int nSock)
    :m_nSock(nSock)
{
}
BhSocket::~BhSocket()
{
    if (m_nSock >= 0)
    {
        ::close(m_nSock);
    }
}
int BhSocket::Attach(int nSock)
{
    if (m_nSock >= 0)
    {
        ::close(m_nSock);
    }
    m_nSock = nSock;
	return 0;
}
int BhSocket::Detach()
{
    int nSock = m_nSock;
    m_nSock = -1;
    return nSock;
}
int BhSocket::GetSocket() const
{
    return m_nSock;
}
int BhSocket::SetNonBlocking()
{
	int nFlags = 0;
    if ((nFlags = fcntl(m_nSock, F_GETFL, 0)) != -1)
    {
		if (fcntl(m_nSock, F_SETFL, nFlags | O_NONBLOCK) != -1)
		{
			return 0;
		}
		return -2;
	}
	return -1;
}
int BhSocket::Init()
{
	return 0;
}
void BhSocket::Clear()
{
	if (m_nSock >= 0)
    {
        ::close(m_nSock);
    }
	m_nSock = -1;
}
