#include <iostream>
#include <stdexcept>
#include <sstream>

#include "Common.hh"
#include "DbTree.hh"

using namespace Ydb;

// Key
Key::Key() : _key(0), _parent(0)
{
}

Key::Key(const DbVal* key, const Type* parent) : _key(key), _parent(parent)
{
}

void 
Key::add(Type* ty)
{
    const char* t = ty->getType();
    Types::iterator i = _types.find(t);
    if (i != _types.end()) {
        std::stringstream ss;
        ss << "Key::add, type allready exists: " << t;
        throw std::invalid_argument(ss.str());
    }
    _types[t] = ty;
}

const DbVal* 
Key::getKey() const
{
    return _key;
}

Type* 
Key::findOrCreate(const std::string& s,
                  Yang::Ast::InteriorNode* schemaRef) 
{
    Type* result;
    Types::const_iterator i = _types.find(s.c_str());
    if (i == _types.end()) {
        result = new Type(s, this, schemaRef);
        _types[result->getType()] = result;
    } else {
        result = i->second;
    }
    return result;
}

Type* 
Key::find(const std::string& s) 
{
    Type* result = 0;
    Types::const_iterator i = _types.find(s.c_str());
    if (i != _types.end()) {
        result = i->second;
    }
    return result;
}

std::vector<Ydb::Vals>& 
Key::getLeafs()
{
    return _leafs;
}

void 
Key::replace(const std::vector<Ydb::Vals>& leafs)
{
    // @todo this most likely leaks
    _leafs = leafs;
}

void
Key::print() const 
{
    std::cerr << "Key:";
    _key->put(std::cerr);
    std::cerr << " {" << std::endl;
    for (unsigned int i = 0; i < _leafs.size(); i++) {
        std::cerr << "attr[" << i << "] = {";
        const std::list<const Ydb::DbVal*>& vals = _leafs[i];
        for (std::list<const Ydb::DbVal*>::const_iterator j = vals.begin();
             j != vals.end();
             ++j) {
            if (j != vals.begin()) std::cerr << ", ";
            (*j)->put(std::cerr);
        }
        std::cerr << "}" << std::endl;
    }
    for (Types::const_iterator i = _types.begin();
         i != _types.end();
         ++i) {
        i->second->print();
    }
    std::cerr << "}" << std::endl;
}

Yang::Ast::InteriorNode& 
Key::getSchemaObj()
{
    if (_parent == 0) {
        Ydb::Common::Error::err(__PRETTY_FUNCTION__, "Should not happen");
    }
    return const_cast<Type*>(_parent)->getSchemaObj();
}

// Type
Type::Type(const std::string& t, 
           const Key* parent, 
           Yang::Ast::InteriorNode* schemaRef) : 
    _type(t), 
    _parent(parent),
    _schemaRef(schemaRef)
{
}

void 
Type::add(Key* key)
{
    const DbVal* k = key->getKey();
    Keys::iterator i = _keys.find(k);
    if (i != _keys.end()) {
        std::stringstream ss;
        ss << "Type::add, key allready exists: " << k;
        throw std::invalid_argument(ss.str());
    }
    _keys[k] = key;
}

Key*
Type::findOrCreate(const DbVal* k, const Type* parent)
{
    Key* result;
    Keys::iterator i = _keys.find(k);
    if (i == _keys.end()) {
        result = new Key(k, parent);
        _keys[k] = result;
    } else {
        delete k;
        result = i->second;
    }
    return result;
}

Key*
Type::find(const DbVal* k)
{
    Key* result = 0;
    Keys::iterator i = _keys.find(k);
    if (i != _keys.end()) {
        result = i->second;
    }
    return result;
}

const char* 
Type::getType() const
{
    return _type.c_str();
}

Keys& 
Type::getKeys() 
{
    return _keys;
}

void 
Type::print() const
{
    std::cerr << "Type:" << _type << " {" << std::endl;
    for (Keys::const_iterator i = _keys.begin();
         i != _keys.end();
         ++i) {
        i->second->print();
    }
    std::cerr << "}" << std::endl;
}
