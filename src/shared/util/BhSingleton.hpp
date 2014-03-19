#ifndef _BHSINGLETON_HPP_
#define _BHSINGLETON_HPP_

#include <thread/BhMutex.hpp>

template <typename T>
class BhSingleton
{
protected:
    BhSingleton(){}
    virtual ~BhSingleton(){}
    BhSingleton(const BhSingleton<T>&);
    BhSingleton<T>& operator=(const BhSingleton<T>&);
public:
    static T& Instance()
    {
        if (NULL == s_instance)
        {
            s_mutex.Lock();
            if (NULL == s_instance)
            {
                s_instance = new T();
            }
            s_mutex.UnLock();
        }
        return *s_instance;
    }
private:
    static BhMutex s_mutex;
    static T* s_instance;
};

#endif
