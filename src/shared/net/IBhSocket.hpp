#ifndef _IBHSOCKET_HPP_
#define _IBHSOCKET_HPP_

class IBhSocket
{
public:
    explicit IBhSocket(int nSock = -1)
		:m_nSock(nSock)
	{
	}
	virtual ~IBhSocket()
	{
		if (m_nSock >= 0)
		{
			::close(m_nSock);
		}
	}
public:
	void Attach(int nSock)
	{
		if (m_nSock >= 0)
		{
			::close(m_nSock);
		}
		m_nSock = nSock;
	}
	int Detach()
	{
		int nSock = m_nSock;
		m_nSock = -1;
		return nSock;
	}
	int GetSocket() const
	{
		return m_nSock;
	}
	bool SetNonBlocking()
	{
		if (m_nSock >= 0)
		{
			return true;
		}
		return false;
	}
public:
	virtual bool Open() = 0;
	virtual void Close() = 0;
protected:
	int m_nSock;
};

#endif
