#ifndef _DB_VALUE_HH_
#define _DB_VALUE_HH_

#include <stdexcept>
#include <sstream>
#include <iostream>
#include <list>

template <class T> struct Dtor 
{
    void operator () (const T* n) {
        delete n;
    }
};

class Bool;
class Singelton;
class String;
class Int64;
class Ref;
class Composite;

class DbVal
{
public:
    virtual bool operator < (const DbVal& val) const = 0;
    virtual bool operator > (const Bool& val) const;
    virtual bool operator > (const Singelton& val) const;
    virtual bool operator > (const String& val) const;
    virtual bool operator > (const Int64& val) const;
    virtual bool operator > (const Ref& val) const;
    virtual bool operator > (const Composite& val) const;
    virtual void put(std::ostream& os) const = 0;
private:
};

class Bool : public DbVal 
{
public:
    Bool(bool b) : _b(b) {}
    bool operator < (const DbVal& val) const {
        return val > *this;
    }
    bool operator > (const Bool& val) const {
        return _b > val._b;
    }
    void put(std::ostream& os) const {
        os << _b;
    }
private:
    bool _b;
};

class Singelton : public DbVal
{
public:
    bool operator < (const DbVal& val) const {
        return val > *this;
    }
    bool operator > (const Singelton& val) const {
        return false;
    }
    void put(std::ostream& os) const {
        os << "Singelton";
    }
};

class String : public DbVal
{
public:
    String(const std::string& s) : _s(s) {}
    bool operator < (const DbVal& val) const {
        return val > *this; 
    }
    bool operator > (const String& val) const {
        return _s > val._s;
    }
    void put(std::ostream& os) const {
        os << _s;
    }
private:
    std::string _s;
};

class Int64 : public DbVal
{
public:
    Int64(int64_t i) : _i(i) {}
    bool operator < (const DbVal& val) const {
        return val > *this;
    }
    bool operator > (const Int64& val) const {
        return _i > val._i;
    }
    void put(std::ostream& os) const {
        os << _i;
    }
private:
    int64_t _i;
};

class Ref : public DbVal
{
public:
private:
};

class Composite : public DbVal
{
public:
private:
};

inline std::ostream&
operator<<(std::ostream& os, const DbVal& v)
{
    v.put(os);
    return os;
}

inline
bool
operator == (const DbVal& l, const DbVal& r)
{
    return !(l < r) && !(r < l);
}
    
struct DbValEq
{
    DbValEq(const DbVal* r) : _r(r) {}
    bool operator() (const DbVal* l) const { return *l == *_r; }
    const DbVal* _r;
};

struct DbValGt
{
    DbValGt(const DbVal* r) : _r(r) {}
    bool operator() (const DbVal* l) const { return (*_r < *l); }
    const DbVal* _r;
};

typedef std::list<const DbVal*> Vals;

#endif
