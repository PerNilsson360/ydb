#ifndef _SCHEMA_HH_
#define _SCHEMA_HH_

#include <cassert>
#include <stdexcept>
#include <sstream>
#include <map>
#include <vector>

#include "Common.hh"

namespace Yang 
{ 
namespace Ast 
{  
class ModuleBase; 
class InteriorNode; 
} 
}

namespace Ydb
{
typedef std::map<std::string, Yang::Ast::ModuleBase*> Mods;

class Modules
{
public:
    void addModule(const std::string& name, Yang::Ast::ModuleBase* module);
    Mods& getModules() { return _modules; }
    Yang::Ast::ModuleBase* findModule(const std::string& name);
    Yang::Ast::InteriorNode* findInteriorNode(const std::string& name);
private:
    Mods _modules;
};

class Schema
{
public:
    Schema(const std::list<std::string>& schemas);
    Mods& getModules() { return _modules.getModules(); }
    Yang::Ast::InteriorNode* findInteriorNode(const std::string& name);
    void print();
private:
    void addSchema(const std::string& fileName);
    Modules  _modules;
};

}

#endif
