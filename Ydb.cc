#include <iostream>

#include <xercesc/dom/DOM.hpp>
#include <xercesc/dom/DOMLSSerializer.hpp>
#include <xercesc/util/XMLString.hpp>
#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/framework/MemBufInputSource.hpp>
#include <xercesc/framework/Wrapper4InputSource.hpp>
#include <xercesc/framework/LocalFileFormatTarget.hpp>

#include "Ydb.hh"
#include "YdbStr.hh"
#include "Node.hh"
#include "Config.hh"
#include "DomUtils.hh"
#include "YangAst.hh"
#include "NetconfException.hh"
#include "NetconfIdentities.hh"

using namespace xercesc;

Ydb::Ydb(const Config& config, DomUtils& domUtils) :
    _config(config),
    _schema(config.getSchemas()),
    _domUtils(domUtils)
{
    DOMDocument* document = domUtils.getDocument();
    _data = document->createElement(NetconfIdentities::DATA.xmlCh());
    read();
}

void
Ydb::merge(const std::string& xml)
{
    /*
    Wrapper4InputSource src(new MemBufInputSource(
				(const XMLByte*)xml.c_str(),
				xml.length(),
				"dummy",
				false));
    DOMDocument* doc = _parser->parse(&src);
    for (DOMNode* n = doc->getFirstChild();
	 n != 0;
	 n = n->getNextSibling()) {
	char* name =  XMLString::transcode(n->getLocalName());
	std::cout << "name : " << name << std::endl;
	// @todo could be leaf not just an interior node
	Yang::Ast::InteriorNode* schemaNode = _schema.findInteriorNode(name);
	XMLString::release(&name);
	std::cout << "schema node: " << schemaNode->getString() << ", "<< schemaNode->getTypeName() << std::endl;
	YDB::Node* node = schemaNode->create(nullptr, n);
	YDB::Key key = node->getKey();
	_roots[key] = node;
    }
    */
}

void
Ydb::merge(DOMNode* domNode, ErrorOption errorOption)
{
    for (DOMNode* n = domNode->getFirstChild();
	 n != 0;
	 n = n->getNextSibling()) {
	if (n->getNodeType() != DOMNode::ELEMENT_NODE) {
	    continue;
	}
	YdbStr name(n->getLocalName());
	Yang::Ast::InteriorNode* schemaNode = _schema.findInteriorNode(name);
	if (schemaNode == nullptr) {
	    std::stringstream ss;
	    ss << "Could not find: " << name << " in schema";
	    throw BadElement(ss, BadElement::Application);
	}
	std::cout << "schema node: " << schemaNode->getString() << ", "<< schemaNode->getTypeName() << std::endl;
	DOMNode* importedNode = _domUtils.getDocument()->importNode(n, true);
	_domUtils.debugPrint("imported node: ", importedNode);
	YDB::Node* node = schemaNode->create(nullptr, _domUtils, importedNode);
	auto i = _roots.find(schemaNode->getString());
	if (i != _roots.end()) {
	    i->second->merge(node);
	} else {
	    YDB::Key key = node->getKey();
	    _roots[key] = node;
	    _data->appendChild(importedNode);
	}
    }
    store();
}

xercesc::DOMNode*
Ydb::get(const std::string& xpath)
{
    return _data;
}

YDB::Node*
Ydb::find(const std::string& name) const
{
    YDB::Node* result = nullptr;
    return result;
}

void
Ydb::store()
{
    const std::string& file = _config.getRunningDbFile();
    LocalFileFormatTarget fileTarget(file.c_str());
    DOMLSOutput* output = _domUtils.getOutput();
    output->setByteStream(&fileTarget);
    DOMLSSerializer* serializer = _domUtils.getSerializer();
    serializer->write(_data, output);
}

void
Ydb::read()
{
    YdbStr running(_config.getRunningDbFile());
    DOMDocument* document = nullptr;
    try {
	DOMDocument* doc = _domUtils.getParser()->parseURI(running.xmlCh());
	if (doc == nullptr) {
	    std::cerr << __PRETTY_FUNCTION__ << ", empty db: " << running
		      << std::endl;
	    return;
	}
	DOMNode* data = _domUtils.findChild(NetconfIdentities::DATA, doc);
	if (data != nullptr) {
	    merge(data, ContinueOnError);
	}
    } catch (const NetconfException& e) {
	std::cerr << "READ: got netconfException" << std::endl;
    } catch (const XMLException& e) {
	YdbStr message(e.getMessage());
	std::cerr << "READ: got exception " << message << std::endl;
    } catch (const DOMException& e) {
	YdbStr message(e.getMessage());
	std::cerr << "READ: got exception " << message << std::endl;
    } catch (...) {
	std::cerr << "READ: Unexpected Exception" << std::endl;
    }
    if (document != nullptr) {
	document->release();
    }
}

