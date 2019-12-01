#include "NetconfException.hh"

using namespace xercesc;

#include "DomUtils.hh"
#include "NetconfIdentities.hh"

// NetconfException
xercesc::DOMNode*
NetconfException::createRpcReply(xercesc::DOMDocument* document,
				 xercesc::DOMNode* rpc) const
{
    DOMNode* reply = document->importNode(rpc, false);
    document->renameNode(reply,
			 NetconfIdentities::NETCONF_XMLNS.xmlCh(),
			 NetconfIdentities::RPC_REPLY.xmlCh());
    return reply;
}


std::string
BadElement::createResponse(DomUtils& domUtils,
			   xercesc::DOMNode* rpc) const
{
    DOMDocument* document = domUtils.newDocument();
    DOMNode* rpcReply = createRpcReply(document, rpc);
    DOMElement* errorTypeElement = document->createElementNS(
	NetconfIdentities::NETCONF_XMLNS.xmlCh(),
	NetconfIdentities::ERROR_TYPE.xmlCh());
    rpcReply->appendChild(errorTypeElement);
    DOMText* text;
    switch (_errorType) {
    case Protocol:
	text = document->createTextNode(NetconfIdentities::PROTOCOL.xmlCh());
	break;
    case Application:
	text = document->createTextNode(NetconfIdentities::APPLICATION.xmlCh());
	break;
    default:
	assert(false && "Unknown errorType getOperationNotSupported ");
    }
    errorTypeElement->appendChild(text);
    DOMElement* errorTagElement = document->createElementNS(
	NetconfIdentities::NETCONF_XMLNS.xmlCh(),
	NetconfIdentities::ERROR_TAG.xmlCh());
    rpcReply->appendChild(errorTagElement);
    text = document->createTextNode(NetconfIdentities::BAD_ELEMENT.xmlCh());
    errorTagElement->appendChild(text);
    std::string result = domUtils.serialize(rpcReply);
    document->release();
    return result;

}


std::string
OperationNotSupported::createResponse(
    DomUtils& domUtils,
    xercesc::DOMNode* rpc) const
{
    DOMDocument* document = domUtils.newDocument();
    DOMNode* rpcReply = createRpcReply(document, rpc);
    DOMElement* errorTypeElement = document->createElementNS(
	NetconfIdentities::NETCONF_XMLNS.xmlCh(),
	NetconfIdentities::ERROR_TYPE.xmlCh());
    rpcReply->appendChild(errorTypeElement);
    DOMText* text;
    switch (_errorType) {
    case Protocol:
	text = document->createTextNode(NetconfIdentities::PROTOCOL.xmlCh());
	break;
    case Application:
	text = document->createTextNode(NetconfIdentities::APPLICATION.xmlCh());
	break;
    default:
	assert(false && "Unknown errorType getOperationNotSupported ");
    }
    errorTypeElement->appendChild(text);
    DOMElement* errorTagElement = document->createElementNS(
	NetconfIdentities::NETCONF_XMLNS.xmlCh(),
	NetconfIdentities::ERROR_TAG.xmlCh());
    rpcReply->appendChild(errorTagElement);
    text = document->createTextNode(
	NetconfIdentities::OPERATION_NOT_SUPPORTED.xmlCh());
    errorTagElement->appendChild(text);
    std::string result = domUtils.serialize(rpcReply);
    document->release();
    return result;
}

// OperationFailed
std::string
OperationFailedcreateResponse(DomUtils& domUtils)
{
}

std::string
OperationFailed::createResponse(DomUtils& domUtils,
				xercesc::DOMNode* rpc) const
{
    DOMDocument* document = domUtils.newDocument();
    DOMNode* rpcReply = createRpcReply(document, rpc);
    DOMElement* errorTypeElement = document->createElementNS(
	NetconfIdentities::NETCONF_XMLNS.xmlCh(),
	NetconfIdentities::ERROR_TYPE.xmlCh());
    rpcReply->appendChild(errorTypeElement);
    DOMText* text;
    switch (_errorType) {
    case Rpc:
	text = document->createTextNode(NetconfIdentities::RPC.xmlCh());
    case Protocol:
	text = document->createTextNode(NetconfIdentities::PROTOCOL.xmlCh());
	break;
    case Application:
	text = document->createTextNode(NetconfIdentities::APPLICATION.xmlCh());
	break;
    default:
	assert(false && "Unknown errorType getOperationFailes ");
    }
    errorTypeElement->appendChild(text);
    DOMElement* errorTagElement = document->createElementNS(
	NetconfIdentities::NETCONF_XMLNS.xmlCh(),
	NetconfIdentities::ERROR_TAG.xmlCh());
    rpcReply->appendChild(errorTagElement);
    text = document->createTextNode(
	NetconfIdentities::OPERATION_FAILED.xmlCh());
    errorTagElement->appendChild(text);
    std::string result = domUtils.serialize(rpcReply);
    document->release();
    return result;
}

std::string
ResourceDenied::createResponse(DomUtils& domUtils) const
{
    return "ResourceDeined";
}

std::string
ResourceDenied::createResponse(DomUtils& domUtils, DOMNode* rpc) const
{
    return "ResourceDeined";
}
