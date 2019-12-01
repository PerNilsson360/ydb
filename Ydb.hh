#ifndef _YDB_HH_
#define _YDB_HH_

#include <map>
#include <string>
#include <memory>

#include <xercesc/dom/DOM.hpp>
#include <xercesc/dom/DOMDocument.hpp>

#include "Key.hh"
#include "Schema.hh"

namespace YDB
{
class Module;
class Node;
}

class DomUtils;
class Config;

class Ydb
{
public:
    enum ErrorOption {
	StopOnError,
	ContinueOnError,
	RollbackOnError
    };
    enum EditOperation {
	Merge,
	Replace,
	Create,
	Delete,
	Remove,
	None
    };
    Ydb(const Config& config, DomUtils& domUtils);
    void merge(const std::string& xml);
    void edit(xercesc::DOMNode* node,
	      EditOperation,
	      ErrorOption errorOption);
    xercesc::DOMNode* get(const std::string& xpath);
private:
    void store();
    void read();
    YDB::Node* find(const std::string& name) const;
    const Config& _config; 
    Schema _schema;
    std::map<YDB::Key, YDB::Node*> _roots;
    xercesc::DOMElement* _data;
    DomUtils& _domUtils;
};
 
#endif
