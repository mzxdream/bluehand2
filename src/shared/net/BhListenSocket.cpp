#include "BhListenSocket.hpp"
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

BhListenSocket::BhListenSocket(unsigned uMaxListen, const std::string& strIP, int nPort)
	:m_uMaxListen(uMaxListen)
    ,m_strIP(strIP)
    ,m_nPort(nPort)
	,m_bReadActive(false)
{
}
BhListenSocket::~BhListenSocket()
{
}
int BhListenSocket::Init()
{
	if (this->BhSocket::Init() != 0)
	{
		return -1;
	}
	if ((m_nSock = ::socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		return -2;
	}
	int nYes = 1;
	if (setsockopt(m_nSock, SOL_SOCKET, SO_REUSEADDR, &nYes, sizeof(nYes)) == -1)
	{
		return -3;
	}
	if (this->SetNonBlocking() != 0)
	{
		return -4;
	}
	struct sockaddr_in addr;
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	if ("" == m_strIP)
	{
		addr.sin_addr.s_addr = INADDR_ANY;
	}
	else
	{
		addr.sin_addr.s_addr = ::inet_addr(m_strIP.c_str());
	}
	addr.sin_port = htons(m_nPort);
	if (::bind(m_nSock, static_cast<struct sockaddr*>(&addr), sizeof(addr)) < 0)
	{
		return -5;
	}
	if (::listen(m_nSock, m_uMaxListen) == -1)
	{
		return -6;
	}

	return 0;
}
void BhListenSocket::Clear()
{
	m_uMaxListen = 0;
	m_strIP = "";
	m_nPort = 0;
	m_bReadActive = false;
	this->BhSocket.Clear();
}
bool BhListenSocket::ReadActvie() const
{
	return m_bReadActive;
}
void BhListenSocket::ReadActive(bool bActive)
{
	m_bReadActive = bActive;
}
unsigned BhListenSocket::MaxListen() const
{
	return m_uMaxListen;
}
int BhListenSocket::Accept(BhTcpSocket& sock)
{
	struct sockaddr_in addr;
	socklen_t len = sizeof(addr);
	int nNewSock = ::accept(m_nSock, static_cast<struct sockaddr*>(&addr), &len);
	if (nNewSock < 0)
	{
		return -1;
	}
	sock.Attach(nNewSock);
	sock.IP(::inet_ntoa(addr.sin_addr));
	sock.Port(::ntohs(addr.sin_port));
	return 0;
}
