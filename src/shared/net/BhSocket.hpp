#ifndef _BHSOCKET_HPP_
#define _BHSOCKET_HPP_

class BhSocket
{
public:
    explicit BhSocket(int nSock = -1);
	virtual ~BhSocket();
private:
	BhSocket(const BhSocket&);
	BhSocket& operator=(const BhSocket&);
public:
	int Attach(int nSock);
	int Detach();
	int GetSocket() const;
	int SetNonBlocking();
public:
	virtual int Init();
	virtual void Clear();
protected:
	int m_nSock;
};

#endif //_BHSOCKET_HPP_
