#ifndef _BHANY_HPP_
#define _BHANY_HPP_

#include <algorithm>
#include <typeinfo>

class BhAny
{
public:
	BhAny()
		:pContent(NULL)
	{
	}
	template<typename T>
	BhAny(const T& obj)
		:pContent(new BhHolder<T>(obj))
	{
	}
	BhAny(const BhAny& obj)
		:pContent(obj.m_pContent ? obj.m_pContent->Clone() : NULL)
	{
	}
	~BhAny
	{
		delete m_pContent;
		m_pContent = NULL;
	}
	template<typename T>
	BhAny& operator=(const T& obj)
	{
		delete m_pContent;
		m_pContent = new BhHolder<T>(obj);
	}
	BhAny& operator=(const BhAny& obj)
	{
		delete m_pContent;
		m_pContent = (obj.m_pContent ? obj.m_pContent->Clone() : NULL);
	}
public:
	BhAny& Swap(BhAny& obj)
	{
		std::swap(m_pContent, obj.m_pContent);
		return *this;
	}
	bool Empty()
	{
		return !m_pContent;
	}
	void Clear()
	{
		delete m_pContent;
		m_pContent = NULL;
	}
	const std::type_info& type()
	{
		return m_pContent ? m_pContent->type() : typeid(void);
	}
private:
	class IBhPlaceHolder
	{
	public:
		IBhPlaceHolder(){}
		virtual ~IBhPlaceHolder(){}
	public:
		virtual const std::type_info& type() const = 0;
		virtual IBhPlaceHolder* Clone() const = 0;
	};
	template<typename T>
	class BhHolder
		:public IBhPlacsHolder
	{
	public:
		BhHolder(const T& obj)
			:m_held(T)
		{
		}
		virtual const std::type_info& type() const
		{
			return typeid(T);
		}
		virtual IBhPlaceHolder* Clone() const
		{
			return new holder(m_held);
		}
	private:
		holder& operator=(const holder&);
	public:
		T m_held;
	};
private:
	template<typename T>
	friend T* BhAnyCast(BhAny *);
	IBhPlaceHolder* m_pContent;
};

template<typename T>
T* BhAnyCast(BhAny* pObj)
{
	return (pObj && pObj->type() == typeid(T)) ? &((static_cast<BhAny::BhHolder<T>*>(pObj->m_pContent))->m_held) : NULL;
}
template<typename T>
const T* BhAnyCast(const BhAny* pObj)
{
	return BhAnyCast<T>(const_cast<BhAny *>(pObj));
}
class BhBadAnyCast
	:public std::bad_cast
{
public:
	virtual const char* what() const throw()
	{
		return "failed conversion using BhAnyCast";
	}
};
template<typename T>
T BhAnyCast(const BhAny& obj)
{
	const T* pHeld = BhAnyCast<T>(&obj);
	if (!pHeld)
	{
		throw BhBadAnyCast();
	}
	return *pHeld;
}
#endif
