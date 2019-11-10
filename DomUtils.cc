#include <iostream>

#include <xercesc/util/XMLString.hpp>
#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/framework/MemBufInputSource.hpp>
#include <xercesc/framework/Wrapper4InputSource.hpp>

#include "YdbStr.hh"

#include "DomUtils.hh"

using namespace xercesc;

DomUtils::DomUtils()
{
    XMLPlatformUtils::Initialize();
    YdbStr ls("LS");
    DOMImplementation *impl = DOMImplementationRegistry::getDOMImplementation(
	ls.xmlCh());
    _parser = ((DOMImplementationLS*)impl)->createLSParser(
	DOMImplementationLS::MODE_SYNCHRONOUS, 0);
    DOMConfiguration* config = _parser->getDomConfig();
    // @todo what should be set really
    // optionally you can set some features on this builder
    if (config->canSetParameter(XMLUni::fgDOMValidate, true)) {
	config->setParameter(XMLUni::fgDOMValidate, true);
    }
    if (config->canSetParameter(XMLUni::fgDOMNamespaces, true)) {
	config->setParameter(XMLUni::fgDOMNamespaces, true);
    }
    if (config->canSetParameter(XMLUni::fgDOMDatatypeNormalization, true)) {
	config->setParameter(XMLUni::fgDOMDatatypeNormalization, true);
    }
    _serializer = ((DOMImplementationLS*)impl)->createLSSerializer();
    // @todo must I get from the serializer
    config = _serializer->getDomConfig();
    // optionally you can set some features on this serializer
    if (config->canSetParameter(XMLUni::fgDOMWRTDiscardDefaultContent, true)) {
	config->setParameter(XMLUni::fgDOMWRTDiscardDefaultContent, true);
    }
    if (config->canSetParameter(XMLUni::fgDOMWRTFormatPrettyPrint, true)) {
	config->setParameter(XMLUni::fgDOMWRTFormatPrettyPrint, true);
    }
    _output = ((DOMImplementationLS*)impl)->createLSOutput();
    _document = newDocument();
}

DomUtils::~DomUtils()
{
    _document->release();
    _parser->release();
    _serializer->release();
}

std::string
DomUtils::serialize(const DOMNode* domNode) const
{
    const XMLCh* xmlCh = _serializer->writeToString(domNode);
    YdbStr result(xmlCh);
    return result;
}

void
DomUtils::debugPrint(const char* info, const DOMNode* domNode) const
{
    std::cout << info << domNode << std::endl;
}

xercesc::DOMDocument*
DomUtils::parse(const char* data, size_t length)
{
    Wrapper4InputSource src(
	new MemBufInputSource((const XMLByte*)data, length, "", false));
    return _parser->parse(&src);
}

DOMDocument*
DomUtils::newDocument()
{
    return parse("", 0);
}

xercesc::DOMNode*
DomUtils::findChild(const YdbStr& name, xercesc::DOMNode* node)
{
    xercesc::DOMNode* result;
    DOMNodeList* nodeList = node->getChildNodes();
    for (int i = 0, size = nodeList->getLength(); i < size; i++) {
	DOMNode* n = nodeList->item(i);
	YdbStr localName(n->getLocalName());
	if (name == localName) {
	    result = n;
	    break;
	}
    }
    return result;
}

void
DomUtils::setText(xercesc::DOMNode* domNode, const YdbStr& text)
{
    DOMNodeList* nodeList = domNode->getChildNodes();
    for (int i = 0, size = nodeList->getLength(); i < size; i++) {
	DOMNode* n = nodeList->item(i);
	if (n->getNodeType() == DOMNode::TEXT_NODE){
	    n->setNodeValue(text.xmlCh());
	}
    }
}

YdbStr
DomUtils::getText(xercesc::DOMNode* domNode)
{
    DOMNode* textNode = nullptr;
    DOMNodeList* nodeList = domNode->getChildNodes();
    for (int i = 0, size = nodeList->getLength(); i < size; i++) {
	DOMNode* n = nodeList->item(i);
	if (n->getNodeType() == DOMNode::TEXT_NODE){
	    textNode = n;
	    break;
	}
    }
    return YdbStr(textNode->getTextContent());
}
