#include "DbVal.hh"

bool 
DbVal::operator > (const Bool& val) const
{
    std::stringstream ss;
    ss << "DbVal::>, can not compare: " << val <<" with: " << *this;
    throw std::invalid_argument(ss.str());
}

bool 
DbVal::operator > (const Singelton& val) const 
{
    std::stringstream ss;
    ss << "DbVal::>, can not compare: " << val <<" with: " << *this;
    throw std::invalid_argument(ss.str());
}

bool 
DbVal::operator > (const String& val) const 
{
    std::stringstream ss;
    ss << "DbVal::>, can not compare: " << val <<" with: " << *this;
    throw std::invalid_argument(ss.str());
}

bool 
DbVal::operator > (const Int64& val) const 
{
    std::stringstream ss;
    ss << "DbVal::>, can not compare: " << val <<" with: " << *this;
    throw std::invalid_argument(ss.str());
}
bool 
DbVal::operator > (const Ref& val) const 
{
    std::stringstream ss;
    ss << "DbVal::>, can not compare: " << val <<" with: " << *this;
    throw std::invalid_argument(ss.str());
}

bool
DbVal::operator > (const Composite& val) const 
{
    std::stringstream ss;
    ss << "DbVal::>, can not compare: " << val <<" with: " << *this;
    throw std::invalid_argument(ss.str());
}
