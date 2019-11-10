#ifndef _DOM_UTILS_HH_
#define _DOM_UTILS_HH_

#include <string>

#include <xercesc/dom/DOM.hpp>
#include <xercesc/dom/DOMLSSerializer.hpp>

class YdbStr;

class DomUtils
{
public:
    DomUtils();
    ~DomUtils();
    std::string serialize(const xercesc::DOMNode* domNode) const;
    void debugPrint(const char* info, const xercesc::DOMNode* domNode) const;
    xercesc::DOMDocument* parse(const char* data, size_t length);
    xercesc::DOMDocument* newDocument();
    xercesc::DOMNode* findChild(const YdbStr& name, xercesc::DOMNode* node);
    void setText(xercesc::DOMNode* domNode, const YdbStr& text);
    static YdbStr getText(xercesc::DOMNode* domNode);
    xercesc::DOMDocument* getDocument() { return _document; }
    xercesc::DOMLSParser* getParser() { return _parser; }
    xercesc::DOMLSSerializer* getSerializer() { return _serializer; }
    xercesc::DOMLSOutput* getOutput() { return _output; }
private:
    xercesc::DOMDocument* _document;
    xercesc::DOMLSParser* _parser;
    xercesc::DOMLSSerializer* _serializer;
    xercesc::DOMLSOutput* _output;
};

#endif
