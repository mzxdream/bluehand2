#ifndef _BHTCPSOCKET_HPP_
#define _BHTCPSOCKET_HPP_

class BhTcpSocket
	:public IBhSocket
{
public:
	BhTcpSocket(int nSock = -1);
	virtual ~BhTcpSocket();
public:
};

#endif
