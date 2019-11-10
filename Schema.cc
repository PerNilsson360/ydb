#include <cstdio>
#include <stdexcept>
#include <iostream>
#include <sstream>

#include "YangAst.hh"
#include "y.tab.hh"
#include "DbTree.hh"
#include "Schema.hh"

extern FILE* yyin;
extern Yang::Ast::ModuleBases* moduleBases;

//Modules
void 
Modules::addModule(const std::string& name, Yang::Ast::ModuleBase* module) 
{
    std::cout << "Proccessing module: " << name << std::endl;
    Mods::iterator i = _modules.find(name);
    if (i != _modules.end()) {
        std::stringstream ss;
        ss << __PRETTY_FUNCTION__  << ": duplicate module: " << name;
        throw std::invalid_argument(ss.str());
    }
    _modules[name] = module;
}

Yang::Ast::ModuleBase* 
Modules::findModule(const std::string& name) 
{
    Yang::Ast::ModuleBase* result = 0;
    Mods::iterator i = _modules.find(name);
    if (i != _modules.end()) {
        result = i->second;
    }
    return result;
}

Yang::Ast::InteriorNode* 
Modules::findInteriorNode(const std::string& name)
{
    Yang::Ast::InteriorNode* result = 0;
    for (Mods::iterator i = _modules.begin();
         i != _modules.end() && result == 0;
         ++i) {
        result = i->second->findInteriorNode(name);
    }
    return result;
}

// Schema
Schema::Schema(const std::list<std::string>& schemas)
{
    if (schemas.size() == 0) {
        throw std::invalid_argument("Db::Db: need at least one schema file.");
    }
    typedef std::list<std::string> Schemas;
    for (Schemas::const_iterator i = schemas.begin(); 
         i != schemas.end(); 
         ++i) {
        addSchema(*i);
    }
    Mods& ms = _modules.getModules();
    for (Mods::iterator i = ms.begin(); i != ms.end(); ++i) {
        i->second->elaborate(this, &_modules);
    }
    for (Mods::iterator i = ms.begin(); i != ms.end(); ++i) {
        i->second->eval(this, &_modules);
    }
}

void
Schema::print()
{
}

extern int yydebug; 

void
Schema::addSchema(const std::string& fileName)
{
    size_t dotPos = fileName.find_first_of(".");
    if (dotPos == std::string::npos) {
        std::stringstream ss;
        ss << "Db::addSchema, could not find '.' in: " << fileName;
        throw std::invalid_argument(ss.str());
    }
    std::string extension = fileName.substr(dotPos + 1);
    if (extension.compare("yang") != 0) {
        std::stringstream ss;
        ss << "Db::addSchema, wrong extension expecting '.yang' in: " << fileName;
        throw std::invalid_argument(ss.str());
    }
    if (!(yyin = fopen(fileName.c_str(), "r"))) {
        std::stringstream ss;
        ss << "Db::addSchema, could not open : " << fileName << " for reading";
        throw std::invalid_argument(ss.str());
    }
    //yydebug = 1;
    moduleBases = 0;
    int rc = yyparse();
    if (rc != 0) {
        std::cerr << "Db::addSchema, failed parsing: " << fileName 
                  << " skipping it." << std::endl;
        return;
    }
    for (Yang::Ast::ModuleBases::iterator i = moduleBases->begin();
         i != moduleBases->end();
         ++i) {
        _modules.addModule((*i)->getString(), *i);
    }
    delete moduleBases;
 }

Yang::Ast::InteriorNode* 
Schema::findInteriorNode(const std::string& name)
{
    return _modules.findInteriorNode(name);
}
