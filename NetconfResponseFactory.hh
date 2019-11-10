#ifndef _NETCONF_RESPONSE_FACTORY_HH_
#define _NETCONF_RESPONSE_FACTORY_HH_

class NetconfResponseFactory
{
public:
    static std::string getOk(unsigned int id) { return _ok; }
private:
    static constexpr const char* _ok =
	"<rpc-reply id=\"106\" xmlns=\"urn:ietf:params:xml:ns:netconf:base:1.0\">\n"
	"<ok/>\n"
	"</rpc-reply>\n";
};

#endif
