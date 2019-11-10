#ifndef _DB_HH_
#define _DB_HH_

#include <string>
#include <map>
#include <vector>
#include <memory>

#include "Common.hh"
#include "DbVal.hh"
#include "Schema.hh"


class Key;
class Obj;

class Db
{
public:
    Db(const std::list<std::string>& schemas);
    ~Db();
    Obj& merge(const std::string& name, const Common::KeyVals& leafs);
    Obj replace(const std::string& name, const Common::KeyVals& leafs);
    // create, error if allready there
    // delete, error if not there
    // remove
    Obj* find(const std::string& name, const Common::KeyVals& leafs);
    void printSchema();
private:
    void init();
    void addSchema(const std::string& fileName);
    std::unique_ptr<Key> _roots;
    Schema _schema;
};

class Obj
{
public:
    Obj(Key* key) : _key(key) {}
    Obj& merge(const std::string& name, const Common::KeyVals& leafs);
    Obj replace(const std::string& name, const Common::KeyVals& leafs);
    void getLeafs(Common::KeyVals& keyVals) const;
    Obj* find(const std::string& name, const Common::KeyVals& leafs);
private:
    Key* getKey() { return _key; }
    Key* _key;
    friend class Db;
};

#endif
