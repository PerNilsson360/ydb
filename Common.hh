#ifndef _COMMON_HH_
#define _COMMON_HH_

#include <stdexcept>
#include <memory>
#include <string>
#include <sstream>
#include <map>
#include <list>

namespace Ydb
{
namespace Common
{

    // @ todo rename to NamedVal
typedef std::multimap<std::string, std::string> KeyVals;

class Error
{
public:
    static void err(const char* func, 
                      const std::string& message, 
                      const char* name = 0) {
        std::stringstream ss;
        ss << func << ": " << message << ": " << (name == 0 ? "" : name);
        throw std::runtime_error(ss.str());
    }
};

class Qname
{
public:
    Qname(const std::string* prefix, const std::string* suffix) :
        _prefix(prefix),
        _suffix(suffix) {}
    const std::string* getPrefix() const {
        return _prefix.get();
    }
    const std::string& getSuffix() const {
        return *(_suffix.get());
    }
    const std::string getString() const {
        std::string result;
        if (_prefix.get() != 0) {
            result += *_prefix;
            result += ":";
        }
        result += *_suffix;
        return result;
    }
private:
    std::unique_ptr<const std::string> _prefix;
    std::unique_ptr<const std::string> _suffix;
};

typedef std::list<const Ydb::Common::Qname*> Qnames;

}
#define YDB_ERROR(e) Ydb::Common::Error::err(__PRETTY_FUNCTION__, e)
#define YDB_N_ERROR(e, n) Ydb::Common::Error::err(__PRETTY_FUNCTION__, e, n)
}
#endif
