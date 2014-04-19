#ifndef _BHSIGNAL_HPP_
#define _BHSIGNAL_HPP_

class IBhDelegate
{
public:
	IBhDelegete(){}
	virtual ~IBhDelegate(){}
public:
	virtual bool IsType(const std::type_info& type) const = 0;
	virtual bool IsEqual(IBhDelegate*) const = 0;
	virtual void* Invoke(void* pParam) = 0;
	virtual IBhDelegate* Copy() const = 0;
};

template<typename T>
class BhDelegate
	:public IBhDelegate
{
	typedef void* (T::*PFnType)(void*);
public:
	explicit MxDelegate(T* pObj, PFnType pFn)
		:m_pObj(pObj)
		,m_pFn(pFn)
	{
	}
public:
	virtual bool IsType(const std::type_info& type) const
	{
		return typeid(BhDelegate<T>) == type;
	}
	virtual bool IsEqual(IBhDelegate* pDelegate) const
	{
		if (pDelegate && pDelegate->IsType(typeid(BhDelegate<T>)))
		{
			BhDelegate<T>* pTmp = dynamic_cast<BhDelegate<T>*>(pDelegate);
			if (pTmp && (pTmp->m_pObj == m_pObj) && (pTmp->m_pFn == m_pFn))
			{
				return true;
			}
		}
		return false;
	}
	virtual void* Invoke(void* pParam)
	{
		if (m_pObj && m_pFn)
		{
			return (m_pObj->*m_pFn)(pParam);
		}
		return NULL;
	}
	virtual IBhDelegate* Copy() const
	{
		return new BhDelegate<T>(*this);
	}
private:
	T* m_pObj;
	PFnType m_pFn;
};


template<>
class BhDelegate<void>
	:public IBhDelegate
{
	typedef void* (*PFnType)(void*);
public:
	explicit MxDelegate(PFnType pFn)
		:m_pFn(pFn)
	{
	}
public:
	virtual bool IsType(const std::type_info& type) const
	{
		return typeid(BhDelegate<void>) == type;
	}
	virtual bool IsEqual(IBhDelegate* pDelegate) const
	{
		if (pDelegate && pDelegate->IsType(typeid(BhDelegate<void>)))
		{
			BhDelegate<void>* pTmp = dynamic_cast<BhDelegate<void>*>(pDelegate);
			if (pTmp && (pTmp->m_pFn == m_pFn))
			{
				return true;
			}
		}
		return false;
	}
	virtual void* Invoke(void* pParam)
	{
		if (m_pFn)
		{
			return m_pFn(pParam);
		}
		return NULL;
	}
	virtual IBhDelegate* Copy() const
	{
		return new BhDelegate<void>(*this);
	}
private:
	PFnType m_pFn;
};

inline IBhDelegate* MakeDelegate(void* (*pFn)(void *))
{
	return new BhDelegate<void>(pFn);
}
template <typename T>
inline IBhDelegate* MakeDelegate(T* pObj, void* (*pFn)(void *))
{
	return new BhDelegate<T>(pObj, pFn);
}

class BhSignal
{
public:
	BhSignal()
		:m_pDelegate(NULL)
	{
	}
	BhSignal(const BhSignal& obj)
	{
		m_pDelegate = obj.m_pDelegate->Copy();
	}
	BhSignal& operator=(const BhSignal& obj)
	{
		if (this != &obj)
		{
			Clear();
			m_pDelegate = obj.m_pDelegate->Copy();
		}
		return *this;
	}
	BhSignal& operator=(IBhDelegate* pObj)
	{
		if (m_pDelegate != pObj)
		{
			Clear();
			m_pDelegate = pObj;
		}
		return *this;
	}
	~BhSignal()
	{
		Clear();
	}
public:
	void* operator()(void* pParam)
	{
		if (m_pDelegate)
		{
			return m_pDelegate->Invoke(pParam);
		}
		return NULL;
	}
	void Clear()
	{
		if (m_pDelegate)
		{
			delete m_pDelegate;
			m_pDelegate = NULL;
		}
	}
private:
	IBhDelegate* m_pDelegate;
};

class BhMultiSignal
{
public:
	BhMultiSignal(){}
	BhMultiSignal(const BhMultiSignal& obj)
	{
		std::list<IBhDelegate*>::const_iterator iter = obj.m_delegateList.begin();
		while (iter != obj.end())
		{
			m_delegateList.push_back((*iter)->Copy());
			++iter;
		}
	}
	BhMultiSignal& operator=(const BhMultiSignal& obj)
	{
		if (this != &obj)
		{
			Clear();
			std::list<IBhDelegate*>::const_iterator iter = obj.m_delegateList.begin();
			while (iter != obj.end())
			{
				m_delegateList.push_back((*iter)->Copy());
				++iter;
			}
		}
		return *this;
	}
	~BhMultiSignal()
	{
		Clear();
	}
public:
	void Clear()
	{
		std::list<IBhDelegate*>::iterator iter = m_delegateList.begin();
		while (iter != m_delegateList.end())
		{
			delete *iter;
			iter = m_delegateList.erase(iter);
		}
	}
	BhMultiSignal& operator+=(IBhDelegate* pDelegate)
	{
		m_delegateList.push_back(pDelegate);
		return *this;
	}
	BhMultiSignal& operator-=(IBhDelegate* pDelegate)
	{
		std::list<IBhDelegate*>::iterator iter = m_delegateList.begin();
		while (iter != m_delegateList.end())
		{
			if ((*iter) && (*iter)->IsEqual(pDelegate))
			{
				if ((*iter) != pDelegate)
				{
					delete *iter;
				}
				iter = m_delegateList.erase(iter);
				break;
			}
			++iter;
		}
	}
	void operator()(void* pParam)
	{
		std::list<IBhDelegate*>::iterator iter = m_delegateList.begin();
		while (iter != m_delegateList.end())
		{
			if (*iter != NULL)
			{
				(*iter)->Invoke(pParam);
			}
			++iter;
		}
	}
	void operator()(void* pParam, std::list<void*>& rstList)
	{
		std::list<IBhDelegate*>::iterator iter = m_delegateList.begin();
		while (iter != m_delegateList.end())
		{
			if (*iter != NULL)
			{
				rstList.push_back((*iter)->Invoke(pParam));
			}
			++iter;
		}
	}
private:
	std::list<IBhDelegate*> m_delegateList;
};
#endif
