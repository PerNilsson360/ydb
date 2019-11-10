#ifndef _NETCONF_RESPONSE_HH_
#define _NETCONF_RESPONSE_HH_

#include <xercesc/dom/DOM.hpp>

class Domutils;

class NetconfResponse
{
public:
    NetconfResponse(DomUtils& domUtils, xercesc::DOMNode* rpc);
    std::string getOk(xercesc::DOMNode* node) const;
private:
    xercesc::DOMNode* createRpcReply(xercesc::DOMDocument* document) const;
    DomUtils& _domUtils;
    xercesc::DOMNode* _rpc;
};

#endif
