#include <stdexcept>
#include <sstream>

#include "YangAst.hh"
#include "DbTree.hh"
#include "Db.hh"

using namespace Ydb;

// class Db
Db::Db(const std::list<std::string>& schemas) :
    _roots(new Key(new Singelton(), 0)),
    _schema(schemas)
{
}

Db::~Db()
{
}

Obj& 
Db::merge(const std::string& name, const Ydb::Common::KeyVals& leafs)
{
}

Obj
Db::replace(const std::string& name, const Ydb::Common::KeyVals& leafs)
{
    Yang::Ast::InteriorNode* n = _schema.findInteriorNode(name);
    if (n == 0) {
        std::stringstream ss;
        ss << __PRETTY_FUNCTION__ << " could not find: " << name 
           << " as a list or container in schema";
        throw std:: invalid_argument(ss.str());
    }
    // @todo fix top level list support
    const DbVal* key = n->createKey(leafs);
    if (key == 0) {
        std::stringstream ss;
        ss << __PRETTY_FUNCTION__ << " could not create key for list: " << name;
        throw std:: invalid_argument(ss.str());
    }
    Type* t = _roots->findOrCreate(name, n);
    Key* k = t->findOrCreate(key, t);
    std::vector<Ydb::Vals> vals;
    n->createLeafs(leafs, vals);
    k->replace(vals);
    return Obj(k);
}

Obj* 
Db::find(const std::string& name, const Ydb::Common::KeyVals& leafs)
{
    const Yang::Ast::Node* node = _schema.findInteriorNode(name);
    if (node == 0) {
        std::stringstream ss;
        ss << __PRETTY_FUNCTION__ << " could not find: " << name 
           << " as a list or container in schema";
        throw std:: invalid_argument(ss.str());
    }
    // @todo add support module level leafs
    const DbVal* key = node->createKey(leafs);
    if (key == 0) {
        std::stringstream ss;
        ss << " could not create key: " << name;
        Common::Error::err(__PRETTY_FUNCTION__, ss.str()); 
    }
    Type* t = _roots->find(name);
    Key* k = t->find(key);
    if (k == 0) {
        std::stringstream ss;
        ss << "could not find key: " << *key;
        Common::Error::err(__PRETTY_FUNCTION__, ss.str());
    }
    return new Obj(k);
}

void 
Db::printSchema()
{
    _schema.print();
}

// class Obj
Obj& 
Obj::merge(const std::string& name, const Ydb::Common::KeyVals& leafs)
{
}

Obj
Obj::replace(const std::string& name, const Ydb::Common::KeyVals& leafs)
{
    Yang::Ast::InteriorNode& schemaObj = _key->getSchemaObj();
    Yang::Ast::InteriorNode* n = schemaObj.findInteriorNode(name);
    if (n == 0) {
        std::stringstream ss;
        ss << __PRETTY_FUNCTION__ << " could not find: " << name 
           << " as a list or container in schema";
        throw std:: invalid_argument(ss.str());
    }
    const DbVal* key = n->createKey(leafs);
    if (key == 0) {
        std::stringstream ss;
        ss << __PRETTY_FUNCTION__ << " could not create key for list: " << name;
        throw std:: invalid_argument(ss.str());
    }
    Type* t = _key->findOrCreate(name, n);
    Key* k = t->findOrCreate(key, t);
    std::vector<Ydb::Vals> vals;
    n->createLeafs(leafs, vals);
    k->replace(vals);
    return Obj(k);
}

void
Obj::getLeafs(Ydb::Common::KeyVals& keyVals) const
{
    const std::vector<Ydb::Vals>& leafs = _key->getLeafs();
    const Yang::Ast::InteriorNode& schemaObj = _key->getSchemaObj();
    for (int i = 0, size = leafs.size(); i < size; i++) {
        const Yang::Ast::LeafBase& leaf = schemaObj.getLeaf(i);
        const std::string& leafName = leaf.getString();
        const Ydb::Vals& vals = leafs[i];
        if (vals.empty()) {
            typedef std::vector<std::string> Defaults;
            const Defaults& ds = leaf.getDefaults();
            for (int i = 0, size = ds.size(); i < size; i++) {
                keyVals.insert(make_pair(leafName, ds[i]));
            }
        } else {
            for (Vals::const_iterator i = vals.begin();
                 i != vals.end();
                 ++i) {
                // @todo stringstreams are slow
                std::stringstream ss;
                ss << **i;
                keyVals.insert(make_pair(leafName, ss.str()));
            }
        }
    }
}

Obj* 
Obj::find(const std::string& name, const Ydb::Common::KeyVals& leafs)
{
    Yang::Ast::InteriorNode& schemaObj = _key->getSchemaObj();
    Yang::Ast::Node* node = schemaObj.findInteriorNode(name);
    if (node == 0) {
        std::stringstream ss;
        ss << __PRETTY_FUNCTION__ << " could not find: " << name 
           << " as a list or container in schema";
        throw std:: invalid_argument(ss.str());
    }
    const DbVal* key = node->createKey(leafs);
    if (key == 0) {
        std::stringstream ss;
        ss << " could not create key: " << name;
        Common::Error::err(__PRETTY_FUNCTION__, ss.str()); 
    }
    Type* t = _key->find(name);
    Key* k = t->find(key);
    if (k == 0) {
        std::stringstream ss;
        ss << "could not find key: " << *key;
        Common::Error::err(__PRETTY_FUNCTION__, ss.str());
    }
    return new Obj(k);
}
