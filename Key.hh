#ifndef _KEY_HH_
#define _KEY_HH_

#include <string>

namespace YDB
{

class Key
{
public:
    Key(const std::string& key) : _key(key) {}
private:
    friend bool operator < (const Key& l, const Key& r);
    friend bool operator == (const Key& l, const Key& r);
    std::string _key;
};

inline
bool
operator < (const Key& l, const Key& r)
{
    return l._key < r._key;
}

inline
bool
operator == (const Key& l, const Key& r)
{
    return l._key == r._key;
}

}
#endif
