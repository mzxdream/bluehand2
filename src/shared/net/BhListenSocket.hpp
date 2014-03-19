#ifndef _BHLISTENSOCKET_HPP_
#define _BHLISTENSOCKET_HPP_

#include "BhTcpSocket.hpp"

class BhListenSocket
	:public IBhSocket
{
public:
	BhListenSocket(int nSock = -1)
		:IBhSocket(nSock)
	{
	}
	virtual ~BhListenSocket(){}
public:
	virtual bool Open(){return true;}
	virtual void Close(){}
	BhTcpSocket* Accept(){}
};

#endif
