#ifndef _NETCONF_MESSAGE_HH_
#define _NETCONF_MESSAGE_HH_

#include <memory>

#include <xercesc/dom/DOM.hpp>

#include "NetconfResponse.hh"

class DomUtils;
class Ydb;
class NetconfOperation;

class NetconfMessage
{
public:
    NetconfMessage(DomUtils& domUtils,
		   const char* message,
		   unsigned int length);
    bool isHello() const { return _hello != nullptr; }
    const std::string& getMessageId() { return _messageId; }
    std::string apply(Ydb& ydb);
private:
    static constexpr const char* RPC_TAG = "rpc";
    static constexpr const char* MESSAGE_ID = "message-id";
    xercesc::DOMNode* findRpc();
    void findMessageId(xercesc::DOMNode* rpc);
    DomUtils& _domUtils;
    std::unique_ptr<NetconfResponse> _netconfResponse;
    xercesc::DOMDocument* _document;
    xercesc::DOMNode* _hello;
    xercesc::DOMNode* _rpc;
    NetconfOperation* _operation;
    std::string _messageId;
};

#endif
