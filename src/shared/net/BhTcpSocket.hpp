#ifndef _BHTCPSOCKET_HPP_
#define _BHTCPSOCKET_HPP_

#include <shared/net/BhSocket.hpp>
#include <shared/util/BhCircleBuf.hpp>

class BhTcpSocket
	:public BhSocket
{
public:
	BhTcpSocket(const std::string& strIP = "", int nPort = 0);
	virtual ~BhTcpSocket();
public:
    virtual int Init();
    virtual void Clear();
	int Attach(int nSock);
	bool ReadActive() const;
	void ReadActive(bool bActive);
	bool WriteActive() const;
	void WriteActive(bool bActive);
	const std::string& IP() const;
	void IP(const std::string& strIP);
	int Port() const;
	void Port(int nPort);
protected:
    BhCircleBuf m_readBuf;
    BhCircleBuf m_writeBuf;
    bool m_bReadActive;
    bool m_bWriteActive;
	std::string m_strIP;
	int m_nPort;
};

#endif
