#include <algorithm>

#include <xercesc/dom/DOM.hpp>

#include "DomUtils.hh"
#include "Config.hh"

const YdbStr Config::YDB_CONFIG("ydb-config");
const YdbStr Config::SCHEMA_PATH("schema-path");
const YdbStr Config::SCHEMA("schema");

using namespace xercesc;

Config::Config(DomUtils& domUtils) :
    _domUtils(domUtils)
{
    char* file = getenv(_ydbConfigEnvVar);
    if (file == nullptr) {
	std::cerr << __PRETTY_FUNCTION__
		  << ", Environment variable: " << _ydbConfigEnvVar
		  << " isNotSet using default:" <<  _ydbConfigFile
		  << std::endl;
    } else {
	_ydbConfigFile = file;
    }
    read();
}

void
Config::read()
{
    DOMDocument* document = nullptr;
    try {
	DOMDocument* doc = _domUtils.getParser()->parseURI(
	    _ydbConfigFile.c_str());
	if (doc == nullptr) {
	    std::cerr << __PRETTY_FUNCTION__ << ", " << _ydbConfigFile
		      << " is empty" << std::endl;
	    return;
	}
	DOMNode* ydbConfig = _domUtils.findChild(YDB_CONFIG, doc);
	if (ydbConfig == nullptr) {
	    std::cerr << __PRETTY_FUNCTION__ << " can not find: "
		      << YDB_CONFIG << " in: " << _ydbConfigFile << std::endl;
	    return;
	}
	_schemas.clear();
	DOMNodeList* nodes = ydbConfig->getChildNodes();
	for (int i = 0, size = nodes->getLength(); i < size; i++) {
	    DOMNode* n = nodes->item(i);
	    YdbStr localName(n->getLocalName());
	    if (localName == SCHEMA_PATH) {
		YdbStr path(_domUtils.getText(n));
		if (!path.str().empty()) {
		    _schemaPath = path;
		}
	    } else if (localName == SCHEMA) {
		YdbStr name(_domUtils.getText(n));
		if (!name.str().empty()) {
		    _schemas.push_back(name);
		}
	    }
	}
	std::transform(
	    _schemas.begin(),
	    _schemas.end(),
	    _schemas.begin(),
	    [&](auto&& n) { return _schemaPath + "/" + n; });
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
