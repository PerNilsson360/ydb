#ifndef _YDB_STR_HH_
#define _YDB_STR_HH_

#include <string>
#include <iostream>

#include <xercesc/util/XMLString.hpp>
#include <xercesc/util/PlatformUtils.hpp>

// @todo use XMLCh as the underlying implementation

class YdbStr
{
public:
    YdbStr(const std::string& s) : _s(s) {}
    YdbStr(const XMLCh* x) {
	if (x == nullptr) {
	    return;
	}
	char* s = xercesc::XMLString::transcode(x);
	_s = s;
	xercesc::XMLString::release(&s);
    }
    const XMLCh* xmlCh() const {
	xercesc::XMLString::transcode(_s.c_str(), _xmlCh, SIZE - 1);
	return _xmlCh;
    }
    const std::string& str() const { return _s; }
    operator const std::string&() const { return _s; }
private:
    static const unsigned int SIZE = 256;
    mutable XMLCh _xmlCh[SIZE];
    std::string _s;
};

inline bool
operator == (const YdbStr& l, const YdbStr& r)
{
    return l.str() == r.str();
}

inline std::ostream&
operator << (std::ostream& os, const YdbStr& s)
{
    os << s.str();
    return os;
}


#endif
