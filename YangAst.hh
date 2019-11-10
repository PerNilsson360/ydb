#ifndef _YANG_AST_HH_
#define _YANG_AST_HH_

#include <cassert>
#include <algorithm>
#include <memory>
#include <list>
#include <vector>
#include <string>
#include <sstream>
#include <iostream>
#include <typeinfo>
#include <cstdint>

#include <xercesc/dom/DOM.hpp>

#include "DbTree.hh"
#include "DbVal.hh"
#include "Common.hh"
#include "Schema.hh"
#include "SchemaTypes.hh"

namespace YDB
{
    class Node;
    class InteriorNode;
}

class DbVal;
class Schema;
class DomUtils;

namespace Yang
{

namespace Ast
{

class Extension;
class Type;
class Unknown;
class LeafBase;
class List;
class Key;
class Augment;
class Grouping;
class Import;

class Node
{
public:
    Node() : _parent(0) {};
    virtual ~Node() {}
    virtual void elaborate(Schema* schema, Node* parent) { _parent = parent; };
    virtual void eval(Schema* schema) {};
    virtual void addExtension(Extension*) { YDB_N_ERROR("Wrong type", getTypeName()); }
    virtual void addImport(Import*) { YDB_N_ERROR("Wrong type", getTypeName()); }
    virtual void addArgument(const std::string*) { YDB_N_ERROR("Wrong type", getTypeName()); }
    virtual void addType(Type*) { YDB_N_ERROR("Wrong type", getTypeName()); }
    // @todo unknowns are no allowed on all nodes but on most add errors where not allowed
    virtual void addUnknown(Unknown* unknown);
    virtual void addMinElements(unsigned int) { YDB_N_ERROR("Wrong type", getTypeName()); }
    virtual void addMaxElements(unsigned int) { YDB_N_ERROR("Wrong type", getTypeName()); }
    virtual void addLeaf(LeafBase*) { YDB_N_ERROR("Wrong type", getTypeName()); }
    // @todo addList should be called addInteriorNode or something
    virtual void addList(InteriorNode*) { YDB_N_ERROR("Wrong type", getTypeName()); }
    virtual void addKey(Key*) { YDB_N_ERROR("Wrong type", getTypeName()); }
    virtual void setMandatory(bool) { YDB_N_ERROR("Wrong type", getTypeName()); }
    // Can be added to the following statements: module, submodule, uses cardinality 0..n
    virtual void addAugment(Augment*) { YDB_N_ERROR("Wrong type", getTypeName()); }
    virtual void addDefault(const std::string&) { YDB_N_ERROR("Wrong type", getTypeName()); }
    virtual void addGrouping(Grouping*) { YDB_N_ERROR("Wrong type", getTypeName()); }
    // accessor methods;
    const Unknown* findUnknown(const std::string& name) const;
    virtual Grouping* findGrouping(const std::string& name) { return _parent->findGrouping(name); }
    virtual const DbVal* createKey(const Common::KeyVals& leafs) const { YDB_N_ERROR("Wrong type", getTypeName()); }
    virtual void createLeafs(const Common::KeyVals& leafs, std::vector<Vals>& vals) const { YDB_N_ERROR("Wrong type", getTypeName()); }
    virtual const Node* findNode(const std::string& name) const { YDB_N_ERROR("Wrong type", getTypeName()); }
    virtual const char* getTypeName() const = 0;
    // crteate data node
    virtual YDB::Node* create(YDB::Node* parent, DomUtils& domUtils, xercesc::DOMNode* dn) const { YDB_N_ERROR("Wrong type", getTypeName()); }
protected:
    std::vector<Unknown*> _unknowns;
    Node* _parent;
};

typedef std::map<std::string, InteriorNode*> NamedInteriorNodes;
typedef std::map<std::string, Import*> Imports;
typedef std::map<std::string, Grouping*> Groupings;
typedef std::map<std::string, LeafBase*> Leafs;


class StringStatement : public Node
{
public:
    StringStatement(const std::string* s) : _s(s) {}
    void elaborate(Schema* schema, Node* parent) { Node::elaborate(schema, parent); };
    const std::string& getString() const { return *_s; }
private:
    std::unique_ptr<const std::string> _s;
};

class IntegerStatement : public Node
{
public:
    IntegerStatement(int64_t i) : _i(i) {}
    void elaborate(Schema* schema, Node* parent) { Node::elaborate(schema, parent); };
protected:
    int64_t _i;
};

class BoolStatement : public Node
{
public:
    BoolStatement(bool b) : _b(b) {}
    void elaborate(Schema* schema, Node* parent) { Node::elaborate(schema, parent); };
protected:
    bool _b;
};

typedef std::vector<Node*> Nodes;

class StringBodyStatement : public Node
{
public:
    StringBodyStatement(const std::string* s, Nodes* body) :
        _s(s), _body(body) {}
    void elaborate(Schema* schema, Node* parent);
    void eval(Schema* schema);
    const std::string& getString() const { return *_s; }
protected:
    std::unique_ptr<const std::string> _s;
    std::unique_ptr<Nodes> _body;
};

class QnameBodyStatement : public Node
{
public:
    QnameBodyStatement(const Common::Qname* qname, Nodes* body) :
        _qname(qname), _body(body) {}
    void elaborate(Schema* schema, Node* parent);
    void eval(Schema* schema);
    const Common::Qname& getQname() const { return *_qname.get(); }
protected:
    std::unique_ptr<const Common::Qname> _qname;
    std::unique_ptr<Nodes> _body;
};

class Import;
class LeafBase;

class ModuleBase : public StringBodyStatement
{
public:
    ModuleBase(const std::string* name, Nodes* body) :
        StringBodyStatement(name, body) {}
    // These two below really needs the virtual keyword
    // since they have different signatures than the node ones
    virtual void elaborate(Schema* schema, Modules* parent);
    virtual void eval(Schema* schema, Modules* parent);
    void addExtension(Extension* extension) { _extensions.push_back(extension); }
    void addImport(Import* import);
    void addLeaf(LeafBase* leaf);
    void addList(InteriorNode* list);
    void addAugment(Augment* augment) { _augment.push_back(augment); }
    void addGrouping(Grouping* grouping);
    Grouping* findGrouping(const std::string& name);
    InteriorNode* findInteriorNode(const std::string& name);
protected:
    NamedInteriorNodes _interiorNodes; // lists, container
    std::vector<LeafBase*> _leafBases; // leaf, leaf-list
    Imports _imports;
    std::vector<Extension*> _extensions;
    std::vector<Augment*> _augment;
    Groupings _groupings;
};

typedef std::vector<ModuleBase*> ModuleBases;

class Module : public ModuleBase
{
public:
    Module(const std::string* name, Nodes* body) :
        ModuleBase(name, body) {}
    void elaborate(Schema* schema, Modules* parent);
    const char* getTypeName() const { typeid(this).name(); }
private:
};

class SubModule : public ModuleBase
{
public:
    SubModule(const std::string* name, Nodes* body) :
        ModuleBase(name, body) {}
    void elaborate(Schema* schema, Modules* parent);
    const char* getTypeName() const { typeid(this).name(); }
private:
};

class YangVersion;
class NameSpace;

class ModuleHeader : public Node
{
public:
    ModuleHeader(const Node* n) : _nodes(1, n) {}
    void add(const Node* n) {
        _nodes.push_back(n);
    }
    const char* getTypeName() const { typeid(this).name(); }
private:
    std::vector<const Node*> _nodes;
};

class YangVersion : public Node
{
public:
    YangVersion(const std::string* name) : _name(name) {}
    const char* getTypeName() const { typeid(this).name(); }
private:
    std::unique_ptr<const std::string> _name;
};

class Prefix : public StringStatement
{
public:
    Prefix(const std::string* prefix) : StringStatement(prefix) {}
    const char* getTypeName() const { typeid(this).name(); }
private:
};

// can be placed on module and submodule 
class Import : public Node 
{
public:
    Import(const std::string* name, 
           const Prefix* prefix, 
           const std::string* revision = 0) :
        _name(name), _prefix(prefix), _revision(revision), _module(0) {}
    void elaborate(Schema* schema, Node* parent);
    virtual void eval(Schema* schema);
    const std::string& getName() const { return *_name; }
    const std::string& getPrefix() const { return _prefix->getString(); }
    const char* getTypeName() const { typeid(this).name(); }
    ModuleBase& getModule() { return *_module; }
private:
    std::unique_ptr<const std::string> _name;
    std::unique_ptr<const Prefix> _prefix;
    std::unique_ptr<const std::string> _revision;
    ModuleBase* _module;
};

class Include : public Node
{
public:
    Include(const std::string*  name, const std::string* revision = 0) :
        _name(name), _revision(revision) {}
    const char* getTypeName() const { typeid(this).name(); }
private:
    std::unique_ptr<const std::string> _name;
    std::unique_ptr<const std::string> _revision;
};

class NameSpace : public Node
{
public:
    NameSpace(const std::string* uri) : _uri(uri) {}
    const char* getTypeName() const { typeid(this).name(); }
private:
    std::unique_ptr<const std::string> _uri;
};

class BelongsTo : public Node
{
public:
    BelongsTo(const std::string* name, const Prefix* prefix) : 
        _name(name) , _prefix(prefix) {}
    const char* getTypeName() const { typeid(this).name(); }
private:
    std::unique_ptr<const std::string> _name;
    std::unique_ptr<const Prefix> _prefix;
};

class Organization : public StringStatement
{
public:
    Organization(const std::string* s) : StringStatement(s) {}
    const char* getTypeName() const { typeid(this).name(); }
};

class Contact : public StringStatement
{
public:
    Contact(const std::string* s) : StringStatement(s) {}
    const char* getTypeName() const { typeid(this).name(); }
};

class Description : public StringStatement
{
public:
    Description(const std::string* s) : StringStatement(s) {}
    const char* getTypeName() const { typeid(this).name(); }
};

class Reference : public StringStatement
{
public:
    Reference(const std::string* s) : StringStatement(s) {}
    const char* getTypeName() const { typeid(this).name(); }
};

class Units : public StringStatement
{
public:
    Units(const std::string* s) : StringStatement(s) {}
    const char* getTypeName() const { typeid(this).name(); }
};

class Revision : public StringBodyStatement
{
public:
    Revision(const std::string* name, Nodes* body = 0) :
        StringBodyStatement(name, body) {}
    const char* getTypeName() const { typeid(this).name(); }
private:
};

class Extension : public StringBodyStatement
{
public:
    Extension(const std::string* name, Nodes* body = 0) :
        StringBodyStatement(name, body), _argument(nullptr) {}
    void elaborate(Schema* schema, Node*);
    void addArgument(const std::string*);
    const std::string* getArgument() { return _argument.get(); }
    const char* getTypeName() const { typeid(this).name(); }
private:
    std::unique_ptr<const std::string> _argument;     
};

class Argument : public StringStatement
{
public:
    Argument(const std::string* prefix) : StringStatement(prefix) {}
    void elaborate(Schema* schema, Node* parent) { 
        StringStatement::elaborate(schema, parent);
        parent->addArgument(new std::string(getString())); 
    }
    const char* getTypeName() const { typeid(this).name(); }
private:
};

class Identity : public StringBodyStatement
{
public:
    Identity(const std::string* name, Nodes* body = 0) : 
        StringBodyStatement(name, body) {}
    const char* getTypeName() const { typeid(this).name(); }
private:
};

class Base : public Node
{
public:
    Base(const Common::Qname* name) : _name(name) {}
    const char* getTypeName() const { typeid(this).name(); }
private:
    std::unique_ptr<const Common::Qname> _name;
};

class Feature : public StringBodyStatement
{
public:
    Feature(const std::string* name, Nodes* body = 0) : 
        StringBodyStatement(name, body) {}
    const char* getTypeName() const { typeid(this).name(); }
private:
};

class IfFeature : public Node
{
public:
    IfFeature(const Common::Qname* name) : _name(name) {}
    const char* getTypeName() const { typeid(this).name(); }
private:
    std::unique_ptr<const Common::Qname> _name;
};

class Typedef : public StringBodyStatement
{
public:
    Typedef(const std::string* name, Nodes* body) : 
        StringBodyStatement(name, body) {}
    void addType(Type*);
    const char* getTypeName() const { typeid(this).name(); }
    void addDefault(const std::string&);
private:
    std::unique_ptr<const std::string> _default;
    std::unique_ptr<const Type> _type;
};

class Type : public QnameBodyStatement
{
public:
    Type(const Common::Qname* qname, Nodes* body = 0) :
        QnameBodyStatement(qname, body) {}
    void elaborate(Schema* schema, Node* parent);
    void addType(Type* type) { _types.push_back(type); }
    const DbVal* create(const std::string& data) const;
    // Memory is owned by this obejct
    const std::string* getDefault() const;
    const char* getTypeName() const { typeid(this).name(); }
private:
    std::unique_ptr<SchemaType> _type;
    std::vector<Type*> _types;
};

class Ranges : public Node
{
public:
    Ranges(const Nodes* ranges) : _ranges(ranges) {}
    const char* getTypeName() const { typeid(this).name(); }
private:
    std::unique_ptr<const Nodes> _ranges;
};

class Length : public Node
{
public:
    Length(const Nodes* lengths, Nodes* body = 0) :
        _lengths(lengths), _body(body)  {}
    const char* getTypeName() const { typeid(this).name(); }
private:
    std::unique_ptr<const Nodes> _lengths;
    std::unique_ptr<const Nodes> _body;
};

class Pattern : public Node
{
public:
    Pattern(const std::string* pattern, Nodes* body = 0) :
        _pattern(pattern), _body(body) {}
    const char* getTypeName() const { typeid(this).name(); }
private:
    std::unique_ptr<const std::string> _pattern;
    std::unique_ptr<const Nodes> _body;
};

class InvertMatch : public Node
{
public:
    const char* getTypeName() const { typeid(this).name(); }
private:
};

class FractionDigits : public Node
{
public:
    FractionDigits(int64_t d) : _d(d) {}
    const char* getTypeName() const { typeid(this).name(); }
private:
    int64_t _d;
};

class Range : public Node
{
public:
    Range(double i) : _min(new double(i)), _max(nullptr) {}
    Range(double min, double max) : 
        _min(new double(min)), _max(new double(max)) {}
    const char* getTypeName() const { typeid(this).name(); }
private:
    std::unique_ptr<double> _min;
    std::unique_ptr<double> _max;
};

class IntegerRange : public Node
{
public:
    IntegerRange(uint64_t i) : _min(new uint64_t(i)), _max(nullptr) {}
    IntegerRange(uint64_t min, uint64_t max) : 
        _min(new uint64_t(min)), _max(new uint64_t(max)) {}
    const char* getTypeName() const { typeid(this).name(); }
private:
    std::unique_ptr<uint64_t> _min;
    std::unique_ptr<uint64_t> _max;
};

class AbsoluteSchemaNodeId : public Node
{
public:
    AbsoluteSchemaNodeId(const Common::Qname* qname) : _qnames(1, qname) {}
    // @todo destructor to avoid mem leak
    void add(const Common::Qname* qname) { _qnames.push_back(qname); }
    const Common::Qnames& getQnames() const { return _qnames; }
    Common::Qnames& getQnames() { return _qnames; }
    const char* getTypeName() const { typeid(this).name(); }
private:
    Common::Qnames _qnames;
};

class DescendantSchemaNodeId : public Node
{
public:
    DescendantSchemaNodeId(const Common::Qname* qname, 
                           AbsoluteSchemaNodeId* asni = 0)
    { 
        _qnames.push_back(qname);
        if (asni != 0) {
            Common::Qnames& v = asni->getQnames();
            _qnames.insert(_qnames.end(), v.begin(), v.end());
            v.clear();
            delete asni;
        }
    }
    // @todo destructor to avoid mem leak
    void elaborate(Schema* schema, Node* parent) { Node::elaborate(schema, parent); }
    void eval(Node* parent) const {}
    const char* getTypeName() const { typeid(this).name(); }
private:
    Common::Qnames _qnames;
};

class Key : public Node
{
public:
    Key(const Common::Qnames* args) :
        _args(args) {}
    ~Key() {
        std::for_each(_args->begin(), _args->end(), Dtor<Common::Qname>());
        delete _args;
        _args = 0;
    }
    void elaborate(Schema* schema, Node* parent);
    const Common::Qnames& getKeys() const {
        return *_args;
    }
    const char* getTypeName() const { typeid(this).name(); }
private:
    const Common::Qnames* _args;
};

class Unique : public Node
{
public:
    Unique(std::list<DescendantSchemaNodeId*>* schemaIds) :
        _schemaIds(schemaIds) {}
    const char* getTypeName() const { typeid(this).name(); }
private:
    std::list<DescendantSchemaNodeId*>* _schemaIds;
};

class Choice : public StringBodyStatement
{
public:
    Choice(const std::string* name, Nodes* body = 0) :
        StringBodyStatement(name, body) {}
    void setMandatory(bool b);
    virtual void addLeaf(LeafBase*);
    void addDefault(const std::string&);
    const char* getTypeName() const { typeid(this).name(); }
private:
    Leafs _leafs; // leaf, leaf-list
    std::unique_ptr<const std::string> _default;
    std::unique_ptr<bool> _mandatory;
};

class Case : public StringBodyStatement
{
public:
    Case(const std::string* name, Nodes* body = 0) :
        StringBodyStatement(name, body) {}
    void addLeaf(LeafBase*);
    void addList(InteriorNode*);
    const char* getTypeName() const { typeid(this).name(); }
private:
    NamedInteriorNodes _interiorNodes; // lists, container
    Leafs _leafs;
    std::vector<LeafBase*> _indexToLeaf;
};

class AnyData : public StringBodyStatement
{
public:
    AnyData(const std::string* name, Nodes* body = 0) :
        StringBodyStatement(name, body) {}
    const char* getTypeName() const { typeid(this).name(); }
private:
};

class AnyXml : public StringBodyStatement
{
public:
    AnyXml(const std::string* name, Nodes* body = 0) :
        StringBodyStatement(name, body) {}
    const char* getTypeName() const { typeid(this).name(); }
private:
};

class Uses : public QnameBodyStatement
{
public:
    Uses(const Common::Qname* qname, Nodes* body = 0) :
        QnameBodyStatement(qname, body) {}
    void elaborate(Schema* schema, Node* parent);
    void eval(Schema* schema);
    void addAugment(Augment* augment) { _augment.push_back(augment); }
    const char* getTypeName() const { typeid(this).name(); }
private:
    std::vector<Augment*> _augment;
};


// +--------------+---------+-------------+
// | substatement | section | cardinality |
// +--------------+---------+-------------+
// | action       | 7.15    | 0..n        |
// | anydata      | 7.10    | 0..n        |
// | anyxml       | 7.11    | 0..n        |
// | case         | 7.9.2   | 0..n        |
// | choice       | 7.9     | 0..n        |
// | container    | 7.5     | 0..n        |
// | description  | 7.21.3  | 0..1        |
// | if-feature   | 7.20.2  | 0..n        |
// | leaf         | 7.6     | 0..n        |
// | leaf-list    | 7.7     | 0..n        |
// | list         | 7.8     | 0..n        |
// | notification | 7.16    | 0..n        |
// | reference    | 7.21.4  | 0..1        |
// | status       | 7.21.2  | 0..1        |
// | uses         | 7.13    | 0..n        |
// | when         | 7.21.5  | 0..1        |
// +--------------+---------+-------------+
class Augment : public Node
{
public:
    Augment(const AbsoluteSchemaNodeId* asni, Nodes* body) :
        _asni(asni), _body(body) {}
    void elaborate(Schema* schema, Node* parent);
    void eval(Schema* schema);
    void addLeaf(LeafBase*);
    void addList(InteriorNode*);
    const char* getTypeName() const { typeid(this).name(); }
private:
    NamedInteriorNodes _interiorNodes; // lists, container
    Leafs _leafs;                      // leafs, leaf-list
    std::unique_ptr<const AbsoluteSchemaNodeId> _asni;
    std::unique_ptr<const Nodes> _body;
};

class When : public StringBodyStatement
{
public:
    When(const std::string* xpath, Nodes* body = 0) :
        StringBodyStatement(xpath, body) {}
    const char* getTypeName() const { typeid(this).name(); }
private:
};

class Rpc : public StringBodyStatement
{
public:
    Rpc(const std::string* name, Nodes* body = 0) :
        StringBodyStatement(name, body) {}
    const char* getTypeName() const { typeid(this).name(); }
private:
};

class Action : public StringBodyStatement
{
public:
    Action(const std::string* name, Nodes* body = 0) :
        StringBodyStatement(name, body) {}
    const char* getTypeName() const { typeid(this).name(); }
private:
};

class Default : public StringStatement
{
public:
    Default(const std::string* s) : StringStatement(s) {}
    void elaborate(Schema* schema, Node* parent);
    const char* getTypeName() const { typeid(this).name(); }
};

class Input : public Node
{
public:
    Input(Nodes* body) : _body(body) {}
    const char* getTypeName() const { typeid(this).name(); }
private:
    std::unique_ptr<const Nodes> _body;
};

class Output : public Node
{
public:
    Output(Nodes* body) : _body(body) {}
    const char* getTypeName() const { typeid(this).name(); }
private:
    std::unique_ptr<const Nodes> _body;
};

class Notification : public StringBodyStatement
{
public:
    Notification(const std::string* name, Nodes* body = 0) :
        StringBodyStatement(name, body) {}
    const char* getTypeName() const { typeid(this).name(); }
private:
};

class Unknown : public QnameBodyStatement
{
public:
    Unknown(const Common::Qname* q, const std::string* s, Nodes* b) :
        QnameBodyStatement(q, b), _s(s) {}
    void elaborate(Schema* schema, Node* parent) { QnameBodyStatement::elaborate(schema, parent); parent->addUnknown(this); }
    const std::string* getString() const { return _s.get(); }
    const char* getTypeName() const { typeid(this).name(); }
private:
    std::unique_ptr<const std::string> _s;
};

class EnumSpec : public StringBodyStatement
{
public:
    EnumSpec(const std::string* name, Nodes* body = 0) :
        StringBodyStatement(name, body) {}
    const char* getTypeName() const { typeid(this).name(); }
private:
};

class Path : public StringStatement
{
public:
    Path(const std::string* s) : StringStatement(s) {}
    const char* getTypeName() const { typeid(this).name(); }
private:
};

class RequireInstance : public BoolStatement
{
public:
    RequireInstance(bool r): BoolStatement(r) {}
    const char* getTypeName() const { typeid(this).name(); }
private:
};

class Bit : public StringBodyStatement
{
public:
    Bit(const std::string* name, Nodes* body = 0) :
        StringBodyStatement(name, body) {}
    const char* getTypeName() const { typeid(this).name(); }
private:
};

class Position : public IntegerStatement
{
public:
    Position(int64_t p) : IntegerStatement(p) {}
    const char* getTypeName() const { typeid(this).name(); }
private:
};

class Status : public StringStatement
{
public:
    Status(const std::string* s) : StringStatement(s) {}
    const char* getTypeName() const { typeid(this).name(); }
private:
};

class Config : public BoolStatement
{
public:
    Config(bool c) : BoolStatement(c) {}
    const char* getTypeName() const { typeid(this).name(); }
private:
};

class Mandatory : public BoolStatement
{
public:
    Mandatory(bool c) : BoolStatement(c) {}
    void elaborate(Schema* schema, Node* parent);
    const char* getTypeName() const { typeid(this).name(); }
private:
};

class Presence : public StringStatement
{
public:
    Presence(const std::string* p) : StringStatement(p) {}
    const char* getTypeName() const { typeid(this).name(); }
private:
};

class UserOrdered : public BoolStatement
{
public:
    UserOrdered(bool u) : BoolStatement(u) {}
    const char* getTypeName() const { typeid(this).name(); }
private:
};

class Must : public StringBodyStatement
{
public:
    Must(const std::string* xpath, Nodes* body = 0) :
        StringBodyStatement(xpath, body) {}
    const char* getTypeName() const { typeid(this).name(); }
private:
};

class ErrorMessage : public StringStatement
{
public:
    ErrorMessage(const std::string* s) : StringStatement(s) {}
    const char* getTypeName() const { typeid(this).name(); }
private:
};

class ErrorAppTag : public StringStatement
{
public:
    ErrorAppTag(const std::string* s) : StringStatement(s) {}
    const char* getTypeName() const { typeid(this).name(); }
private:
};

class MinElements : public IntegerStatement
{
public:
    MinElements(int64_t v) : IntegerStatement(v) {}
    void elaborate(Schema* schema, Node* parent) { IntegerStatement::elaborate(schema, parent); parent->addMinElements(_i); }
    const char* getTypeName() const { typeid(this).name(); }
private:
};

class MaxElements : public IntegerStatement
{
public:
    MaxElements(int64_t v) : IntegerStatement(v) {}
    void elaborate(Schema* schema, Node* parent) { IntegerStatement::elaborate(schema, parent); parent->addMaxElements(_i); }
    const char* getTypeName() const { typeid(this).name(); }
private:
};

class Value : public IntegerStatement
{
public:
    Value(int64_t v) : IntegerStatement(v) {}
    const char* getTypeName() const { typeid(this).name(); }
private:
};


// placed on module, submodule, container, list, rpc, input, output
// action, notification 
class Grouping : public StringBodyStatement
{
public:
    Grouping(const std::string* name, Nodes* body = 0) :
        StringBodyStatement(name, body) {}
    virtual void elaborate(Schema* schema, Node* parent);
    void addLeaf(LeafBase*);
    void addList(InteriorNode*);
    const char* getTypeName() const { typeid(this).name(); }
    NamedInteriorNodes& getInteriorNodes() { return _interiorNodes; }
    Leafs& getLeafs() { return _leafs; }
private:
    NamedInteriorNodes _interiorNodes; // lists, container
    Leafs _leafs;                      // leaf, leaf-list
};

class Leaf;

    // base class for interior nodes in the instance tree
    // i.e Container and List
class InteriorNode : public StringBodyStatement
{
public:
    InteriorNode(const std::string* name, Nodes* body) : StringBodyStatement(name, body) {}
    void addLeaf(LeafBase* leaf);
    void addList(InteriorNode* list);
    void addMinElements(unsigned int m) { _minElements.reset(new unsigned int(m)); }
    void addMaxElements(unsigned int m) { _maxElements.reset(new unsigned int(m)); }
    void createLeafs(const Common::KeyVals& leafs, std::vector<Vals>& vals) const;
    const LeafBase& getLeaf(unsigned int index) const { return *_indexToLeaf.at(index); }
    InteriorNode* findInteriorNode(const std::string& name);
    const Node* findNode(const std::string& name) const; 
    const char* getTypeName() const { typeid(this).name(); }
protected:
    std::unique_ptr<unsigned int> _minElements;
    std::unique_ptr<unsigned int> _maxElements;
    NamedInteriorNodes _interiorNodes; // lists, container
    Leafs _leafs;
    std::vector<LeafBase*> _indexToLeaf;
};

class Container : public InteriorNode
{
public:
    Container(const std::string* name, Nodes* body = 0) : InteriorNode(name, body) {}
    void elaborate(Schema* schema, Node* parent);
    void addGrouping(Grouping* grouping);
    Grouping* findGrouping(const std::string& name);
    const char* getTypeName() const { typeid(this).name(); }
    YDB::Node* create(YDB::Node* n, DomUtils& domUtils, xercesc::DOMNode* dn) const;
private:
    Groupings _groupings;
};

// A class that encapsulates common behaviour
// of leaf and leaf-list 
class LeafBase : public StringBodyStatement
{
public:
    LeafBase(const std::string* name, Nodes* body) :
        StringBodyStatement(name, body) {}
    void elaborate(Schema* schema, Node* parent);
    bool validCardinality(unsigned int i) const;
    unsigned int getMinElements() const { return (_minElements.get() == 0 ? 0 : *_minElements); }
    void addType(Type* type);
    const Type& getType() const { return *_type; }
    void assignIndex(unsigned int index) { _index = index; }
    unsigned int getIndex() const { return _index; }
    const std::vector<std::string>& getDefaults() const { return _defaults; }
    const char* getTypeName() const { typeid(this).name(); }
protected:
    std::unique_ptr<Type> _type;
    std::vector<std::string> _defaults;
    std::unique_ptr<unsigned int> _minElements;
    std::unique_ptr<unsigned int> _maxElements;
    unsigned int _index;
};

class Leaf : public LeafBase
{
public:
    Leaf(const std::string* name, Nodes* body) : LeafBase(name, body) {}
    void eval(Schema* schema);
    void setMandatory(bool b);
    void addDefault(const std::string& d);
    YDB::Node* create(YDB::Node* parent, DomUtils& domUtils, xercesc::DOMNode* dn) const;
    const char* getTypeName() const { typeid(this).name(); }
private:
    std::unique_ptr<bool> _mandatory;
};

class LeafList : public LeafBase
{
public:
    LeafList(const std::string* name, Nodes* body) : LeafBase(name, body) {}
    void eval(Schema* schema);
    void addMinElements(unsigned int m) { _minElements.reset(new unsigned int(m)); }
    void addMaxElements(unsigned int m) { _maxElements.reset(new unsigned int(m)); }
    void addDefault(const std::string& d) { _defaults.push_back(d); }
    const char* getTypeName() const { typeid(this).name(); }
private:
};

class List : public InteriorNode
{
public:
    List(const std::string* name, Nodes* body) : InteriorNode(name, body) {} 
    void elaborate(Schema* schema, Node* parent);
    void eval(Schema* schema);
    void addKey(Key* key);
    const DbVal* createKey(const Common::KeyVals& leafs) const;
    void addGrouping(Grouping* grouping);
    Grouping* findGrouping(const std::string& name);
    const char* getTypeName() const { typeid(this).name(); }
private:
    std::unique_ptr<Key> _key;
    Groupings _groupings;
};

}
}

#endif 
