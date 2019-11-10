#include "YdbStr.hh"
#include "DomUtils.hh"

#include "NetconfIdentities.hh"
#include "NetconfResponse.hh"

using namespace xercesc;

NetconfResponse::NetconfResponse(DomUtils& domUtils, DOMNode* rpc) :
    _domUtils(domUtils),
    _rpc(rpc)
{
}

std::string
NetconfResponse::getOk(xercesc::DOMNode* node) const
{
    DOMDocument* document = _domUtils.newDocument();
    DOMNode* rpcReply = createRpcReply(document);
    if (node != nullptr) {
	DOMNode* dataNode = document->importNode(node, true);
	rpcReply->appendChild(dataNode);
    }
    std::string result = _domUtils.serialize(rpcReply);
    document->release();
    return result;
}

DOMNode*
NetconfResponse::createRpcReply(xercesc::DOMDocument* document) const
{
    DOMNode* reply = document->importNode(_rpc, false);
    document->renameNode(reply,
			 NetconfIdentities::NETCONF_XMLNS.xmlCh(),
			 NetconfIdentities::RPC_REPLY.xmlCh());
    return reply;
}
