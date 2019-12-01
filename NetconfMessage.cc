#include <stdexcept>
#include <iostream>
#include <sstream>

#include <xercesc/dom/DOM.hpp>
#include <xercesc/util/XMLString.hpp>
#include <xercesc/framework/MemBufInputSource.hpp>
#include <xercesc/framework/Wrapper4InputSource.hpp>


#include "YdbStr.hh"
#include "DomUtils.hh"
#include "NetconfMessage.hh"
#include "NetconfException.hh"
#include "NetconfOperation.hh"
#include "NetconfResponse.hh"
#include "NetconfIdentities.hh"

using namespace xercesc;

NetconfMessage::NetconfMessage(DomUtils& domutils,
			       const char* message,
			       unsigned int length) :
    _domUtils(domutils)
{
    std::string m(message, length);
    std::size_t len = m.find("]]>]]>");
    if (len == std::string::npos) {
	std::stringstream ss;
	ss << "Can not find message termination sequence.";
	throw OperationFailed(ss, OperationFailed::Protocol);
    }
    _document = _domUtils.parse(message, length);
    if (_document == nullptr) {
	std::stringstream ss;
	ss << "Unkown parsing error";
	throw OperationFailed(ss, OperationFailed::Protocol);
    }
    _hello = _domUtils.findChild(NetconfIdentities::HELLO,
				 _document);
    if (_hello != nullptr) {
	return;
    }
    _rpc = findRpc();
    findMessageId(_rpc);
    _netconfResponse.reset(new NetconfResponse(_domUtils, _rpc));
    _operation = NetconfOperation::create(_rpc);
}

std::string
NetconfMessage::apply(Ydb& ydb)
{
    std::string reply;
    try {
	DOMNode* node = _operation->apply(ydb);
	reply = _netconfResponse->getOk(node);
    } catch (const NetconfException& e) {
	std::cerr << __PRETTY_FUNCTION__ << ", exception "
		  << e.what() << std::endl;
	reply = e.createResponse(_domUtils, _rpc);
    }
    return reply;
}

DOMNode*
NetconfMessage::findRpc()
{
    DOMNode* result = nullptr;
    for (DOMNode* n = _document->getFirstChild();
	 n != 0;
	 n = n->getNextSibling()) {
	YdbStr name(n->getLocalName());
	if (static_cast<std::string>(name) == RPC_TAG) {
	    result = n;
	    break;
	}
    }
    if (result == nullptr) {
	std::stringstream ss;
	ss << "Could not find rpc node";
	throw OperationFailed(ss, OperationFailed::Protocol);
    }
    return result;
}

void
NetconfMessage::findMessageId(DOMNode* rpc)
{
    if (rpc->getNodeType() != DOMNode::ELEMENT_NODE) {
	std::stringstream ss;
	ss << "Failed to cast rpc to ELEMENT_NODE";
	throw OperationFailed(ss, OperationFailed::Protocol);
    }
    DOMElement* elem = static_cast<DOMElement*>(rpc);
    const XMLCh* idName = XMLString::transcode(MESSAGE_ID);
    const XMLCh* idValue = elem->getAttribute(idName);
    if (idValue == nullptr) {
	std::stringstream ss;
	ss << "Could not find message-id";
	throw OperationFailed(ss, OperationFailed::Protocol);
    }
    _messageId = YdbStr(idValue); 
}
