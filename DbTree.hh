#ifndef _DB_TREE_HH_
#define _DB_TREE_HH_

#include <cstring>
#include <string>
#include <vector>
#include <map>

#include "DbVal.hh"

namespace Yang
{
namespace Ast
{
class InteriorNode;
}
}

struct CStrLess {
    bool operator() (const char* l, const char* r) const {
        return strcmp(l, r) < 0;
    }
};
class Type;
typedef std::map<const char*, Type*, CStrLess> Types;

struct DbValLess {
    bool operator() (const DbVal* l, const DbVal* r) const {
        return *l < *r;
    }
};
class Key;
typedef std::map<const DbVal*, Key*, DbValLess> Keys;

class Key
{
public:
    Key();
    Key(const DbVal* key, const Type* parent);
    void add(Type* ty);
    const DbVal* getKey() const;
    Type* findOrCreate(const std::string& s, 
                       Yang::Ast::InteriorNode* schemaRef);
    Type* find(const std::string& s);
    std::vector<Vals>& getLeafs();
    void replace(const std::vector<Vals>& leafs);
    void print() const;
    Yang::Ast::InteriorNode& getSchemaObj();
protected:
    const DbVal* _key;
    std::vector<Vals> _leafs;
    Types _types;
    const Type* _parent;
};

class Type
{
public:
    Type(const std::string& t, 
         const Key* parent, 
         Yang::Ast::InteriorNode* schemaRef);
    void add(Key* key);
    Key* findOrCreate(const DbVal* k, const Type* parent);
    Key* find(const DbVal* k);
    const char* getType() const;
    Keys& getKeys();
    void print() const;
    Yang::Ast::InteriorNode& getSchemaObj() {
        return *_schemaRef;
    }
protected:
    std::string _type;
    Yang::Ast::InteriorNode* _schemaRef;
    Keys _keys;
    const Key* _parent;
};    

#endif
