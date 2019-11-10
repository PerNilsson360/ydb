#include <sstream>
#include <iostream>

#include "DomUtils.hh"
#include "YdbStr.hh"
#include "Ydb.hh"
#include "NetconfException.hh"
#include "NetconfOperation.hh"
#include "NetconfResponse.hh"
#include "NetconfIdentities.hh"
#include "NetconfResponseFactory.hh"

using namespace xercesc;

// NetconfOperation
NetconfOperation::NetconfOperation(DOMNode* rpc) :
    _rpc(rpc)
{
}

NetconfOperation*
NetconfOperation::create(DOMNode* rpc)
{
    try {
	if (!rpc->hasChildNodes()) {
	    std::stringstream ss;
	    ss << "<rpc>: does not have child nodes";
	    throw OperationFailed(ss, OperationFailed::Protocol);
	}
	for (DOMNode* n = rpc->getFirstChild();
	     n != 0;
	     n = n->getNextSibling()) {
	    if (n->getNodeType() != DOMNode::ELEMENT_NODE) {
		continue;
	    }
	    char* name =  XMLString::transcode(n->getLocalName());
	    if (strcmp(name, GET) == 0) {
	       return new Get(n);
	    } else if (strcmp(name, GET_CONFIG) == 0) {
		return new GetConfig(n);
	    } else if (strcmp(name, EDIT_CONFIG) == 0) {
		return new EditConfig(n);
	    } else if (strcmp(name, DELETE_CONFIG) == 0) {
		return new DeleteConfig(n);
	    } else if (strcmp(name, COPY_CONFIG) == 0) {
		return new CopyConfig(n);
	    } else if (strcmp(name, LOCK) == 0) {
		return new Lock(n);
	    } else if (strcmp(name, UNLOCK) == 0) {
		return new Unlock(n);
	    } else if (strcmp(name, CLOSE_SESSION) == 0) {
		return new CloseSession(n);
	    } else if (strcmp(name, KILL_SESSION) == 0) {
		return new KillSession(n);
	    }
	    std::stringstream ss;
	    ss << "Could not find operation: " << name << std::endl;
	    XMLString::release(&name);
	    throw OperationFailed(ss, OperationFailed::Protocol);
	}
    } catch (const DOMException& toCatch) {
	char* message = XMLString::transcode(toCatch.msg);
	std::stringstream ss;
	ss << "Failed to get operation: " << message << std::endl;
	XMLString::release(&message);
	throw OperationFailed(ss, OperationFailed::Protocol);
    }
}

// Get
Get::Get(DOMNode* rpc) :
    NetconfOperation(rpc)
{   
}

DOMNode*
Get::apply(Ydb& ydb)
{
    std::string xpath;		// @todo implement xpath filter
    return ydb.get(xpath);
}

// GetConfig
GetConfig::GetConfig(DOMNode* rpc) :
    NetconfOperation(rpc)
{
}

DOMNode*
GetConfig::apply(Ydb& ydb)
{
    std::stringstream ss;
    ss << "get-config not supported";
    throw OperationNotSupported(ss, OperationNotSupported::Protocol);
}

// EditConfig
EditConfig::EditConfig(DOMNode* rpc) :
    NetconfOperation(rpc)
{
}

DOMNode*
EditConfig::apply(Ydb& ydb)
{
    DOMNode* configNode = nullptr;
    DOMNode* errorOptionNode = nullptr;
    for (DOMNode* n = _rpc->getFirstChild();
	 n != 0;
	 n = n->getNextSibling()) {
	if (n->getNodeType() != DOMNode::ELEMENT_NODE) {
	    continue;
	}
	YdbStr name(n->getLocalName());
	if (name == NetconfIdentities::CONFIG) {
	    configNode = n;
	    break;
	} else if (name == NetconfIdentities::ERROR_OPTION) {
	    if (errorOptionNode != nullptr) {
		std::stringstream ss;
		ss << __PRETTY_FUNCTION__ << " mulltiple error-options ";
		throw BadElement(ss, BadElement::Protocol);
	    }
	    errorOptionNode = n;
	}
    }
    if (configNode != nullptr) {
	Ydb::ErrorOption errorOption = getErrorOption(errorOptionNode);
	ydb.merge(configNode, errorOption);
    } else {
	std::cerr << "edit-config: did not contain config node"
		  << std::endl;
    }
    return nullptr;
}

Ydb::ErrorOption
EditConfig::getErrorOption(xercesc::DOMNode* errorOptionNode)
{
    Ydb::ErrorOption result = Ydb::StopOnError;
    if (errorOptionNode == nullptr) {
	return result;
    }
    YdbStr option = DomUtils::getText(errorOptionNode);
    if (option == NetconfIdentities::CONTINUE_ON_ERROR) {
	result = Ydb::ContinueOnError;
    } else if (option == NetconfIdentities::ROLLBACK_ON_ERROR) {
	result = Ydb::RollbackOnError;
    }
    return result;
}

// CopyConfig
CopyConfig::CopyConfig(DOMNode* rpc) :
    NetconfOperation(rpc)
{
}

DOMNode*
CopyConfig::apply(Ydb& ydb)
{
    std::stringstream ss;
    ss << "copy-config not supported";
    throw OperationNotSupported(ss, OperationNotSupported::Application);
}

// DeleteConfig
DeleteConfig::DeleteConfig(DOMNode* rpc) :
    NetconfOperation(rpc)
{
}

DOMNode*
DeleteConfig::apply(Ydb& ydb)
{
    std::stringstream ss;
    ss << "delete-config not supported";
    throw OperationNotSupported(ss, OperationNotSupported::Application);
}

// Lock
Lock::Lock(DOMNode* rpc) :
    NetconfOperation(rpc)
{
}

DOMNode*
Lock::apply(Ydb& ydb)
{
    std::stringstream ss;
    ss << "lock not supported";
    throw OperationNotSupported(ss, OperationNotSupported::Application);
}

// unlock
Unlock::Unlock(DOMNode* rpc) :
    NetconfOperation(rpc)
{
}

DOMNode*
Unlock::apply(Ydb& ydb)
{
    std::stringstream ss;
    ss << "unllock not supported";
    throw OperationNotSupported(ss, OperationNotSupported::Application);
}


// CloseSession
CloseSession::CloseSession(DOMNode* rpc) :
    NetconfOperation(rpc)
{
}

DOMNode*
CloseSession::apply(Ydb& ydb)
{
    std::stringstream ss;
    ss << "close-session not supported";
    throw OperationNotSupported(ss, OperationNotSupported::Application);
}

// KillSession
KillSession::KillSession(DOMNode* rpc) :
    NetconfOperation(rpc)
{
}

DOMNode*
KillSession::apply(Ydb& ydb)
{
    std::stringstream ss;
    ss << "kill-session not supported";
    throw OperationNotSupported(ss, OperationNotSupported::Application);
}

