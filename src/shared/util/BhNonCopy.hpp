#ifndef _BHNONCOPY_HPP_
#define _BHNONCOPY_HPP_
class BhNonCopy
{
protected:
    BhNonCopy(){}
    ~BhNonCopy(){}
private:
    BhNonCopy(const BhNonCopy&);
    BhNonCopy& operator=(const BhNonCopy&);
};
#endif
