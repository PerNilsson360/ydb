#include <cassert>
#include <string>
#include <list>
#include <iostream>
#include <fstream>

#include "Db.hh"
#include "Ydb.hh"

void
readFile(const std::string& name, std::string& result)
{
    std::string line;
    std::ifstream file(name);
    if (file.is_open()) {
	while (getline(file, line)){
	    result.append(line);
	}
	file.close();
    }
}

void
oneContainer()
{
    std::list<std::string> schemas = { "test_models/one-container.yang" };
    Ydb ydb(schemas);
    std::string dataFile("test_models/one-container-data.xml");
    std::string data;
    readFile(dataFile, data);
    ydb.merge(data);
}


void
dbValTest()
{
    std::unique_ptr<String> v1(new String("foo"));
    std::unique_ptr<String> v2(new String("foo"));
    assert(!(*v1 < *v2) && "v1 < v2");
    assert(!(*v1 > *v2) && "v1 > v2");
    assert(*v1 == *v2 && "v1 != v2");
    DbValEq eq(v1.get());
    assert(eq(v2.get()) && "v1 is not eq v2");
}

void 
simpleReplaceTest() 
{
    std::list<std::string> files(1, "test_models/simple_test.yang");
    Db db(files);
    Common::KeyVals keyVals;
    keyVals.insert(std::make_pair("bar", "xyz"));
    Obj fooObj = db.replace("foo", keyVals);
    Obj bazObj = fooObj.replace("baz", keyVals);
    std::unique_ptr<Obj> obj(db.find("foo", keyVals));
    Common::KeyVals leafs;
    obj->getLeafs(leafs);
    assert(leafs.size() == 1 && "'foo' does not have correct number of leafs");
    assert(leafs.count("bar") == 1 && "'foo' should have one bar leaf");
    assert(leafs.find("bar")->second == "xyz" && "foo's bar leaf should have value xyz");
    leafs.clear();
    obj.reset(obj->find("baz", keyVals));
    obj->getLeafs(leafs);
    assert(leafs.size() == 1 && "'baz' does not have correct number of leafs");
    assert(leafs.count("bar") == 1 && "'baz' should have one bar leaf");
    assert(leafs.find("bar")->second == "xyz" && "baz's bar leaf should have value xyz");
}

void
leafCardinality()
{
    std::list<std::string> files(1, "test_models/leaf.yang");
    Db db(files);
    Common::KeyVals keyVals;
    keyVals.insert(std::make_pair("baz1", "xyz"));
    try {
        (void) db.replace("foo", keyVals);
        assert(false && "can do replace without key");
    } catch (std::runtime_error& e) {}
    keyVals.insert(std::make_pair("bar", "xyz"));
    keyVals.insert(std::make_pair("bar", "xyz"));
    try {
        (void) db.replace("foo", keyVals);
        assert(false && "can do replace with key that has cardinality 2");
    } catch (std::runtime_error& e) {}
    keyVals.clear();
    keyVals.insert(std::make_pair("bar", "xyz"));
    try {
        (void) db.replace("foo", keyVals);
        assert(false && "can do replace with missing mandatory leaf");
    } catch (std::runtime_error& e) {}
    keyVals.insert(std::make_pair("baz1", "xyz"));
    keyVals.insert(std::make_pair("baz2", "xyz"));
    keyVals.insert(std::make_pair("baz2", "xyz"));
    try {
        (void) db.replace("foo", keyVals);
        assert(false && "can do replace with leaf with cardinality 2");
    } catch (std::runtime_error& e) {}
    keyVals.clear();
    keyVals.insert(std::make_pair("bar", "xyz"));
    keyVals.insert(std::make_pair("baz1", "xyz"));
    keyVals.insert(std::make_pair("baz2", "xyz"));
    Obj fooObj = db.replace("foo", keyVals);    
}

void
leafDefault()
{
    std::list<std::string> files(1, "test_models/leaf.yang");
    Db db(files);
    Common::KeyVals leafs;
    leafs.insert(std::make_pair("bar", "xyz"));
    leafs.insert(std::make_pair("baz1", "xyz"));
    (void) db.replace("foo", leafs);
    std::unique_ptr<Obj> obj(db.find("foo", leafs));
    leafs.clear();
    obj->getLeafs(leafs);
    assert(leafs.size() == 3 && "'foo' does not have correct number of leafs");
    assert(leafs.count("bar") == 1 && "'foo' should have one bar leaf");
    assert(leafs.find("bar")->second == "xyz" && "foo's bar leaf should have value xyz");
    assert(leafs.count("baz1") == 1 && "'foo' should have one baz1 leaf");
    assert(leafs.find("baz1")->second == "xyz" && "foo's bar leaf should have value xyz");
    assert(leafs.count("baz2") == 0 && "'foo' should have one baz2 leaf");
    assert(leafs.count("baz3") == 1 && "'foo' should have one baz3 leaf");
    assert(leafs.find("baz3")->second == "a default" && "foo's baz3 leaf should have value 'a default'");
}

void
leafListCardinality()
{
    std::list<std::string> files(1, "test_models/leaf_list.yang");
    Db db(files);
    Common::KeyVals leafs;
    leafs.insert(std::make_pair("bar", "xyz"));
    leafs.insert(std::make_pair("baz1", "xyz1"));
    leafs.insert(std::make_pair("baz3", "xyz1"));
    leafs.insert(std::make_pair("baz3", "xyz2"));
    try {
    (void) db.replace("foo", leafs);
    assert(false && "min-elements on baz1 does not work");
    } catch (const std::runtime_error& e) {}
    leafs.insert(std::make_pair("baz1", "xyz2"));
    leafs.insert(std::make_pair("baz2", "xyz1"));
    leafs.insert(std::make_pair("baz2", "xyz2"));
    leafs.insert(std::make_pair("baz2", "xyz3"));
    leafs.insert(std::make_pair("baz2", "xyz4"));
    try {
        (void) db.replace("foo", leafs);
        assert(false && "min-elements on baz2 does not work");
    } catch (const std::runtime_error& e) {}
    leafs.clear();
    leafs.insert(std::make_pair("bar", "xyz"));
    leafs.insert(std::make_pair("baz1", "xyz1"));
    leafs.insert(std::make_pair("baz1", "xyz2"));
    leafs.insert(std::make_pair("baz3", "xyz1"));
    try {
        (void) db.replace("foo", leafs);
        assert(false && "min-elements on baz3 does not work");
    } catch (const std::runtime_error& e) {}
    leafs.insert(std::make_pair("baz3", "xyz2"));
    leafs.insert(std::make_pair("baz3", "xyz3"));
    leafs.insert(std::make_pair("baz3", "xyz4"));
    try {
        (void) db.replace("foo", leafs);
        assert(false && "max-elements on baz3 does not work");
    } catch (const std::runtime_error& e) {}
    leafs.insert(std::make_pair("baz4", "xyz"));
    leafs.insert(std::make_pair("baz4", "xyz"));
    leafs.clear();
    leafs.insert(std::make_pair("bar", "xyz"));
    leafs.insert(std::make_pair("baz1", "xyz1"));
    leafs.insert(std::make_pair("baz1", "xyz2"));
    leafs.insert(std::make_pair("baz3", "xyz1"));
    leafs.insert(std::make_pair("baz3", "xyz2"));
}

void
leafListDuplicate()
{
    std::list<std::string> files(1, "test_models/leaf_list_simple.yang");
    Db db(files);
    Common::KeyVals leafs;
    leafs.insert(std::make_pair("bar", "xyz"));
    leafs.insert(std::make_pair("baz1", "xyz1"));
    leafs.insert(std::make_pair("baz1", "xyz1"));
    try {
        (void) db.replace("foo", leafs);
        assert(false && "can add duplicate leaf-list values");
    } catch (const std::runtime_error& e) {}
}

void
containerSimple()
{
    std::list<std::string> files(
	1,
	"test_models/container_simple.yang");
    Db db(files);
}

void 
test() 
{
    std::list<std::string> files(1, "test_models/test.yang");
    files.push_back("test_models/ietf-yang-types.yang");
    Db db(files);
}

void 
rfc6536() 
{
    std::list<std::string> files(1, "test_models/rfc6536.yang");
    files.push_back("test_models/ietf-yang-types.yang");
    Db db(files);
}

void 
rfc6991() 
{
    std::list<std::string> files(1, "test_models/rfc6991.yang");
    Db db(files);
}

void 
ianaCryptHash()
{
    std::list<std::string> files(1, "test_models/iana_crypt_hash.yang");
    Db db(files);
}

void 
rfc7317()
{
    std::list<std::string> files(1, "test_models/rfc7317.yang");
    files.push_back("test_models/ietf-yang-types.yang");
    files.push_back("test_models/ietf-netconf-acm.yang");
    files.push_back("test_models/ietf-inet-types.yang");
    files.push_back("test_models/iana_crypt_hash.yang");
    Db db(files);
    db.printSchema();
}

void 
rfcs()
{
    std::list<std::string> files(1, "test_models/rfc6536.yang");
    files.push_back("test_models/rfc6991.yang");
    files.push_back("test_models/iana_crypt_hash.yang");
    files.push_back("test_models/rfc7317.yang");
    files.push_back("test_models/ietf-inet-types.yang");
    Db db(files);
}

int 
main(int argc, char **argv)
{
    oneContainer();
    /*
    dbValTest();
    simpleReplaceTest();
    leafCardinality();
    leafDefault();
    leafListCardinality();
    leafListDuplicate();
    containerSimple();
    test();
    rfc6536();
    rfc6991();
    ianaCryptHash();
    rfc7317();
    rfcs();
    */
    return 0;
}
