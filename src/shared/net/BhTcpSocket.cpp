#include "BhTcpSocket.hpp"
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

BhTcpSocket::BhTcpSocket(const std::string& strIP, int nPort)
	:m_strIP(strIP)
	,m_nPort(nPort)
	,m_bReadActive(false)
	,m_bWriteActive(false)
{
}
int BhTcpSocket::Init()
{
	if (this->BhSocket::Init() != 0)
	{
		return -1;
	}
	if ((m_nSock = ::socket(AF_INET, SOCKET_STREAM, 0)) == -1)
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
	return 0;
}
void BhTcpSocket::Clear()
{
	m_bReadActive = false;
	m_bWriteActive = false;
	m_strIP = "";
	m_nPort = "";
	this->BhSocket::Clear();
}
int BhTcpSocket::Attach(int nSock)
{
	if (this->BhSocket::Attach(nSock) != 0)
	{
		return -1;
	}
	if (this->SetNonBlocking(nSock) != 0)
	{
		return -2;
	}
	m_bReadActive = false;
	m_bWriteActive = false;
	m_strIP = "";
	m_nPort = 0;
	return 0;
}
bool BhTcpSocket::ReadActive() const
{
	return m_bReadActive;
}
void BhTcpSocket::ReadActive(bool bActive)
{
	m_bReadActive = bActive;
}
bool BhTcpSocket::WriteActive() const
{
	return m_bWriteActive();
}
void BhTcpSocket::WriteActive(bool bActive)
{
	m_bWriteActive = bActive;
}
const std::string& BhTcpSocket::IP() const
{
	return m_strIP;
}
void BhTcpSocket::IP(const std::string& strIP)
{
	m_strIP = strIP;
}
int BhTcpSocket::Port() const
{
	return m_nPort;
}
void BhTcpSocket::Port(int nPort)
{
	m_nPort = nPort;
}
