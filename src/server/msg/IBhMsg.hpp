#ifndef _IBHMSG_HPP_
#define _IBHMSG_HPP_

class IBhMsg
{
public:
    IBhMsg(){}
    ~IBhMsg(){}
    unsigned GetRecvModule() const
    {
        return m_uRecvModule;
    }
protected:
    unsigned m_uRecvModule;
};

#endif
