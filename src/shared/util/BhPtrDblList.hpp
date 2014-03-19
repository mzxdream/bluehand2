#ifndef _BHPTRDBLLIST_HPP_
#define _BHPTRDBLLIST_HPP_

#include <list>
#include <iostream>

template <typename T>
class BhPtrDblList
{
public:
    BhPtrDblList(){}
    ~BhPtrDblList()
    {
        Clear();
    }
public:
    bool Pop(T*& pBuf)
    {
        if (m_popList.empty())
        {
            pBuf = NULL;
            return false;
        }
        pBuf = m_popList.front();
        m_popList.pop_front();
        return true;
    }
    void Push(T* pBuf)
    {
        m_pushList.push_back(pBuf);
    }
    std::size_t GetPushSize() const
    {
        return m_pushList.size();
    }
    std::size_t GetPopSize() const
    {
        return m_popList.size();
    }
    void Exchange()
    {
        m_popList.swap(m_pushList);
    }
    void Clear()
    {
        typename std::list<T*>::iterator iter;
        for (iter = m_popList.begin(); iter != m_popList.end();)
        {
            delete *iter;
            iter = m_popList.erase(iter);
        }
        for (iter = m_pushList.begin(); iter != m_pushList.end();)
        {
            delete *iter;
            iter = m_pushList.erase(iter);
        }
    }
private:
    std::list<T*> m_popList;
    std::list<T*> m_pushList;
};

#endif
