#ifndef _BHSOCKET_HPP_
#define _BHSOCKET_HPP_

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <unistd.h>
#include <string>
#include "../BhNonCopy.hpp"

enum class BhSockType{TCP = 0, UDP = 1};

class BhSocket
    :public BhNonCopy
{
public:
    explicit BhSocket(int nSock = -1)
        :m_nSock(nSock)
    {
    }
    virtual ~BhSocket()
    {
        Destroy();
    }
public:
    void Attach(int nSock)
    {
        Destroy();
        m_nSock = nSock;
    }
    int Detach()
    {
        int nSock = m_nSock;
        m_nSock = -1;
        return nSock;
    }
    bool Create(BhSockType type)
    {
        if (BhSockType::TCP == type)
        {
            m_nSock = ::socket(PF_INET, SOCK_STREAM, 0);
        }
        else if (BhSockType::UDP == type)
        {
            m_nSock = ::socket(PF_INET, SOCK_DGRAM, 0);
        }
        return m_nSock >= 0;
    }
    bool Bind(const std::string& strIP = "", int nPort = 0)
    {
        return Bind(m_nSock, strIP, nPort);
    }
    static bool Bind(int nSock, const std::string& strIP = "", int nPort = 0)
    {
        struct sockaddr_in addr;
        addr.sin_family = AF_INET;
        addr.sin_port = ::htons(nPort);
        if (strIP.empty())
        {
            addr.sin_addr.s_addr = INADDR_ANY;
        }
        else
        {
            addr.sin_addr.s_addr = ::inet_addr(strIP.c_str());
        }
        return ::bind(nSock, (struct sockaddr *)&addr, sizeof(addr)) >= 0;
    }
    bool Listen(int nCount)
    {
        return Listen(m_nSock, nCount);
    }
    static bool Listen(int nSock, int nCount)
    {
        return ::listen(nSock, nCount) >= 0;
    }
    bool SetNonBlock()
    {
        return SetNonBlock(m_nSock);
    }
    bool SetNonBlock(int nSock)
    {
        int nFlags = 0;
        if ((nFlags = ::fcntl(nSock, F_GETFL, 0)) != -1)
        {
            if (::fcntl(nSock, F_SETFL, nFlags|O_NONBLOCK) != -1)
            {
                return true;
            }
        }
        return false;
    }
    int GetSocket() const
    {
        return m_nSock;
    }
    void Destory()
    {
        if (m_nSock >= 0)
        {
            ::close(m_nSock);
            m_nSock = -1;
        }
    }
private:
    int m_nSock;
};

#endif
