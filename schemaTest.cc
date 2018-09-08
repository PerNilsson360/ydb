#include <cassert>
#include <string>
#include <list>
#include <iostream>

#include "Schema.hh"
#include "YangAst.hh"

using namespace Ydb;

void 
extension()
{
    std::list<std::string> files(1, "test_models/extension.yang");
    Schema schema(files);
    const Yang::Ast::InteriorNode* i = schema.findInteriorNode("foo");
    assert(i != 0);
    const Yang::Ast::Unknown* u = i->findUnknown("name");
    assert(u != 0);
    const std::string* argument = u->getString();
    assert(argument != 0);
    assert(*argument == "eventFoo");
}

void
simpleAugment()
{
    std::list<std::string> files(1, "test_models/simple_augment.yang");
    Schema schema(files);
    Yang::Ast::InteriorNode* i = schema.findInteriorNode("A");
    assert(i != 0);
    i = i->findInteriorNode("B");
    assert(i != 0);
    i = i->findInteriorNode("C");
    assert(i != 0);
    const Yang::Ast::Unknown* u = i->findUnknown("immName");
    assert(u != 0);
    const std::string* argument = u->getString();
    assert(argument != 0);
    assert(*argument == "AC");
}

void
simpleUses()
{
    std::list<std::string> files(1, "test_models/simple_uses.yang");
    Schema schema(files);
}

void
simple()
{
    std::list<std::string> files(1, "test_models/simple_schema.yang");
    Schema schema(files);
}

int 
main(int argc, char **argv)
{
    simpleAugment();
    simple();
    simpleUses();
    extension();
    return 0;
}
