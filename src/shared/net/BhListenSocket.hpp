#ifndef _BHLISTENSOCKET_HPP_
#define _BHLISTENSOCKET_HPP_

#include <shared/net/BhTcpSocket.hpp>

class BhListenSocket
	:public BhSocket
{
public:
	explicit BhListenSocket(unsigned uMaxListen, const std::string& strIP = "", int nPort = 0);
	virtual ~BhListenSocket();
public:
	virtual int Init();
	virtual void Clear();
	bool ReadActive() const;
	void ReadActive(bool bActive);
	unsigned MaxListen() const;
	int Accept(BhTcpSocket& sock);
private:
    unsigned m_uMaxListen;
    std::string m_strIP;
    int m_nPort;
    bool m_bReadActive;
};

#endif
