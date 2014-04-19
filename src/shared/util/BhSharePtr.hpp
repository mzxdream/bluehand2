#ifndef _BHSHAREPTR_HPP_
#define _BHSHAREPTR_HPP_

class BhUserCount
{
public:
    BhUserCount()
        :m_pCount(new unsigned(1))
    {
    }
    ~BhUserCount()
    {
        if (0 == --*m_pCount)
		{
			delete m_pCount;
			m_pCount = NULL;
		}
    }
	BhUserCount(const BhUserCount& obj):m_pCount(obj.m_pCount)
	{
		++*m_pCount;
	}
	bool Reattach(const BhUserCount& obj)
	{
		++*obj.m_pCount;
		if (0 == --*m_pCount)
		{
			delete m_pCount;
			m_pCount = obj.m_pCount;
			return true;
		}
		m_pCount = obj.m_pCount;
		return false;
	}
	bool IsOnly()
	{
		return 1 == *m_pCount;
	}
private:
	BhUserCount& operator=(const BhUserCount& obj);
private:
	unsigned int* m_pCount;
};

template<typename T>
class BhSharePtr
{
public:
	BhSharePtr():m_pObj(NULL){}
	~BhSharePtr()
	{
		if (m_count.IsOnly())
		{
			delete m_pObj;
			m_pObj = NULL;
		}
	}
	BhSharePtr(const MxSharePtr& obj):m_pObj(obj.m_pObj),m_count(obj.m_count)
	{
	}
	BhSharePtr(const T& obj):m_pObj(new T(obj))
	{
	}
	BhSharePtr(T*& pObj)
	{
		m_pObj = pObj;
	}
	BhSharePtr& operator= (const BhSharePtr& obj)
	{
		if (m_count.Reattach(obj.m_count))
		{
			delete m_pObj;
		}
		m_pObj = obj.m_pObj;
		return *this;
	}
	T* operator->()
	{
		return this->m_pObj;
	}
	T& operator*()
	{
		return *(this->m_pObj);
	}
private:
	BhUserCount m_count;
	T* m_pObj;
};


#endif
