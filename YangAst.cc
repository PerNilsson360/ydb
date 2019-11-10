#include "Node.hh"
#include "Schema.hh"
#include "DomUtils.hh"

#include "YangAst.hh"

namespace Yang
{
    
namespace Ast
{
// Node

void 
Node::addUnknown(Unknown* unknown)
{ 
    _unknowns.push_back(unknown); 
}

const Unknown* 
Node::findUnknown(const std::string& name) const
{
    // @todo handle namespaces and stuff
    const Unknown* result = 0;
    for (size_t i = 0, size = _unknowns.size(); i < size; i++) {
        const Unknown* u = _unknowns[i];
        const Common::Qname& qname = u->getQname();
        if (qname.getSuffix() == name) {
            result = u;
            break;
        }
    }
    return result;
}

// StringBodyStatement
void 
StringBodyStatement::elaborate(Schema* schema, Node* parent) 
{
    Node::elaborate(schema, parent);
    if (_body.get() != 0) {
        for (Nodes::iterator i = _body->begin(); i != _body->end(); ++i) {
            (*i)->elaborate(schema, this);
        }
    }
};

void 
StringBodyStatement::eval(Schema* schema)
{
    if (_body.get() != 0) {
        for (Nodes::iterator i = _body->begin(); i != _body->end(); ++i) {
            (*i)->eval(schema);
        }
    }
};

// QnameBodyStatement
void 
QnameBodyStatement::elaborate(Schema* schema, Node* parent) 
{
    Node::elaborate(schema, parent);
    if (_body.get() != 0) {
        for (Nodes::iterator i = _body->begin(); i != _body->end(); ++i) {
            (*i)->elaborate(schema, this);
        }
    }
};

void 
QnameBodyStatement::eval(Schema* schema)
{
    if (_body.get() != 0) {
        for (Nodes::iterator i = _body->begin(); i != _body->end(); ++i) {
            (*i)->eval(schema);
        }
    }
};


// ModuleBase
void 
ModuleBase::elaborate(Schema* schema, Modules* parent)
{
    StringBodyStatement::elaborate(schema, 0);
}

void 
ModuleBase::eval(Schema* schema, Modules* parent)
{
    StringBodyStatement::eval(schema);
}

void 
ModuleBase::addImport(Import* import)
{
    const std::string& name = import->getName();
    Imports::iterator i = _imports.find(name);
    if (i != _imports.end()) {
        std::stringstream ss;
        ss << "duplicate imported module: "  << name 
           << " in module: " << getString();
        YDB_ERROR(ss.str().c_str());
    }
    _imports[name] =  import;
}

void 
ModuleBase::addLeaf(LeafBase* leaf)
{
    _leafBases.push_back(leaf);
}

void
ModuleBase::addList(InteriorNode* list)
{
    const std::string& name = list->getString();
    NamedInteriorNodes::iterator i = _interiorNodes.find(name);
    if (i != _interiorNodes.end()) {
        YDB_ERROR("duplicate 'list' definiton");
    }
    _interiorNodes[name] =  list;
}

void 
ModuleBase::addGrouping(Grouping* grouping)
{
    const std::string& name = grouping->getString();
    Groupings::iterator i = _groupings.find(name);
    if (i != _groupings.end()) {
        YDB_N_ERROR("duplicate grouping definiton", name.c_str());
    }
    _groupings[name] =  grouping;
}
 
Grouping* 
ModuleBase::findGrouping(const std::string& name)
{
    Grouping* result = 0;
    Groupings::iterator i = _groupings.find(name);
    if (i != _groupings.end()) {
        result = i->second;
    }
    // @todo check submodules
    if (result == 0) {
        for (Imports::iterator i = _imports.begin(); 
             i != _imports.end();
             ++i) {
            ModuleBase& module = i->second->getModule();
            result = module.findGrouping(name);
            if (result != 0) break;
        }
    }
    return result;
}

InteriorNode*
ModuleBase::findInteriorNode(const std::string& name)
{
    InteriorNode* result = 0;
    NamedInteriorNodes::iterator i = _interiorNodes.find(name);
    if (i != _interiorNodes.end()) {
        result = i->second;
    }
    return result;
}

// Module
void
Module::elaborate(Schema* schema, Modules* parent)
{
    // modules needs to be added first so imports can find them
    //parent->addModule(*_s, this);
    ModuleBase::elaborate(schema, parent);
}

// SubModule
void 
SubModule::elaborate(Schema* schema, Modules* parent)
{
    ModuleBase::elaborate(schema, parent);
}

// Import
void
Import::elaborate(Schema* schema, Node* parent) 
{
    Node::elaborate(schema, parent);
    parent->addImport(this);
    const std::string& name = *_name;
    typedef std::map<std::string, Yang::Ast::ModuleBase*> Modules;
    Modules& modules = schema->getModules();
    Modules::iterator i = modules.find(name);
    if (i == modules.end()) {
        YDB_N_ERROR("Could not find import", name.c_str());
    }
    _module = i->second;
}

void 
Import::eval(Schema* schema)
{
}

// Extension
void 
Extension::elaborate(Schema* schema, Node* parent)
{
    StringBodyStatement::elaborate(schema, parent);
    parent->addExtension(this);
}

void 
Extension::addArgument(const std::string* a) 
{ 
    if (_argument.get() != 0) {
        YDB_N_ERROR("More than one arguemnt statement", getTypeName());
    }
    _argument.reset(a);
}

// Key
void 
Key::elaborate(Schema* schema, Node* parent)
{
    Node::elaborate(schema, parent);
    parent->addKey(this);
}

// Choice
void 
Choice::setMandatory(bool b)
{
    if (_mandatory.get() != 0) {
        YDB_ERROR("Multiple mandatory statememts");
    }
    _mandatory.reset(new bool(b));
}

void 
Choice::addLeaf(LeafBase* leaf)
{
    const std::string& name = leaf->getString();
    Leafs::iterator i = _leafs.find(name);
    if (i != _leafs.end()) {
        YDB_N_ERROR("Duplicate leafs", getTypeName());
    }
    _leafs[name] = leaf;
}

void
Choice::addDefault(const std::string& d)
{
    if (_default.get() != 0) {
        YDB_N_ERROR("More than one default statement", getTypeName());
    }
    _default.reset(new std::string(d));
}

// Case
void 
Case::addLeaf(LeafBase* leaf)
{
    const std::string& name = leaf->getString();
    Leafs::iterator i = _leafs.find(name);
    if (i != _leafs.end()) {
        YDB_N_ERROR("Duplicate leafs", getTypeName());
    }
    leaf->assignIndex(_leafs.size());
    _leafs[name] = leaf;
    _indexToLeaf.push_back(leaf);
}

void
Case::addList(InteriorNode* list)
{
    const std::string& name = list->getString();
    NamedInteriorNodes::iterator i = _interiorNodes.find(name);
    if (i != _interiorNodes.end()) {
        YDB_N_ERROR("Duplicate lists", getTypeName());
    }
    _interiorNodes[name] =  list;
}

// Mandatory
void
Mandatory::elaborate(Schema* schema, Node* parent)
{
    BoolStatement::elaborate(schema, parent);
    parent->setMandatory(_b);
}

// Typedef
void 
Typedef::addType(Type* t)
{
    if (_type.get() != 0) {
        YDB_N_ERROR("More than one type statement", getTypeName());
    }
    _type.reset(t);
}

void
Typedef::addDefault(const std::string& d)
{
    if (_default.get() != 0) {
        YDB_N_ERROR("More than one default statement", getTypeName());
    }
    _default.reset(new std::string(d));
}

// Type
void 
Type::elaborate(Schema* schema, Node* parent)
{
    QnameBodyStatement::elaborate(schema, parent);
    const std::string& type = _qname->getString();
    if (type == "binary") {
        _type.reset(new SchemaBinary());
    } else if (type == "bits") {
        _type.reset(new SchemaBits());
    } else if (type == "boolean") {
        _type.reset(new SchemaBoolean());
    } else if (type == "decimal64") {
        _type.reset(new SchemaDecimal64());
    } else if (type == "empty") {
        _type.reset(new SchemaEmpty());
    } else if (type == "enumeration") {
        _type.reset(new SchemaEnumeration());
    } else if (type == "identityref") {
        _type.reset(new SchemaIdentityref());
    } else if (type == "instance-identifier") {
        _type.reset(new SchemaInstanceIdentifier());
    } else if (type == "int8") {
        _type.reset(new SchemaInt8());
    } else if (type == "int16") {
        _type.reset(new SchemaInt16());
    } else if (type == "int32") {
        _type.reset(new SchemaInt32());
    } else if (type == "int64") {
        _type.reset(new SchemaInt64());
    } else if (type == "leafref") {
        _type.reset(new SchemaLeafref());
    } else  if (type == "string") {
        _type.reset(new SchemaString());
    } else  if (type == "uint8") {
        _type.reset(new SchemaUInt8());
    } else if (type == "uint16") {
        _type.reset(new SchemaInt16());
    } else if (type == "uint32") {
        _type.reset(new SchemaInt32());
    } else if (type == "uint64") {
        _type.reset(new SchemaInt64());
    } else if (type == "union") {
        _type.reset(new SchemaUnion());
    } else {
        std::cout << "Not a supported type: "  << type << std::endl;
    }
    parent->addType(this);
}

const DbVal* 
Type::create(const std::string& data) const
{
    return _type->create(data);
}

const std::string* 
Type::getDefault() const
{
    // @todo implement
    return 0;
}
// Grouping
void 
Grouping::elaborate(Schema* schema, Node* parent)
{
    StringBodyStatement::elaborate(schema, parent);
    parent->addGrouping(this);
}

void 
Grouping::addLeaf(LeafBase* leaf)
{
    const std::string& name = leaf->getString();
    Leafs::iterator i = _leafs.find(name);
    if (i != _leafs.end()) {
        YDB_N_ERROR("Duplicate leafs", getTypeName());
    }
    _leafs[name] = leaf;
}

void 
Grouping::addList(InteriorNode* list)
{
    const std::string& name = list->getString();
    NamedInteriorNodes::iterator i = _interiorNodes.find(name);
    if (i != _interiorNodes.end()) {
        YDB_N_ERROR("Duplicate lists", getTypeName());
    }
    _interiorNodes[name] =  list;
}

// InteriorNode
void 
InteriorNode::addLeaf(LeafBase* leaf)
{
    const std::string& name = leaf->getString();
    Leafs::iterator i = _leafs.find(name);
    if (i != _leafs.end()) {
        YDB_N_ERROR("Duplicate leafs", getTypeName());
    }
    leaf->assignIndex(_leafs.size());
    _leafs[name] = leaf;
    _indexToLeaf.push_back(leaf);
}

void
InteriorNode::addList(InteriorNode* list)
{
    const std::string& name = list->getString();
    NamedInteriorNodes::iterator i = _interiorNodes.find(name);
    if (i != _interiorNodes.end()) {
        YDB_N_ERROR("Duplicate lists", getTypeName());
    }
    _interiorNodes[name] =  list;
}

void 
InteriorNode::createLeafs(const Common::KeyVals& leafs, 
                  std::vector<Vals>& vals) const
{
    // create DB leaf objects fro´m strings
    vals.resize(_leafs.size());
    for (Common::KeyVals::const_iterator i = leafs.begin();
         i != leafs.end();
         ++i) {
        const std::string& leafName = i->first;
        Leafs::const_iterator j = _leafs.find(leafName);
        if (j == _leafs.end()) {
            YDB_N_ERROR("Could not find leaf", leafName.c_str());
        }
        const LeafBase* leaf = j->second;
        const Type& type = leaf->getType();
        Vals& vs = vals[leaf->getIndex()];
        const DbVal* val = type.create(i->second);
        // @todo it must be possible to skip one find and do it faster
        Vals::iterator k = std::find_if(vs.begin(), vs.end(), DbValEq(val));
        if (k != vs.end()) {
            std::stringstream ss;
            ss << "duplicate value in leaf: " << *val;
            YDB_N_ERROR(ss.str(), leafName.c_str());
        }
        k = std::find_if(vs.begin(), vs.end(), DbValGt(val));
        vs.insert(k, val);
    }
    // validate cardinalities
    for (int i = 0, size = _indexToLeaf.size(); i < size; ++i) {
        const LeafBase* leaf = _indexToLeaf[i];
        unsigned int cardinality = vals[i].size();
        if (!leaf->validCardinality(cardinality)) {
            std::stringstream ss;
            ss << "Invalid cardinality: " <<  cardinality;
            YDB_N_ERROR(ss.str(), leaf->getString().c_str());
        }
    }
}

InteriorNode*
InteriorNode::findInteriorNode(const std::string& name)
{
    InteriorNode* result = 0;
    NamedInteriorNodes::iterator i = _interiorNodes.find(name);
    if (i != _interiorNodes.end()) {
        result = i->second;
    }
    return result;
}

const Node*
InteriorNode::findNode(const std::string& name) const
{
    const Node* result = 0;
    NamedInteriorNodes::const_iterator i = _interiorNodes.find(name);
    if (i != _interiorNodes.end()) {
        result = i->second;
	return result;
    }
    Leafs::const_iterator j = _leafs.find(name);
    if (j != _leafs.end()) {
        result = j->second;
	return result;
    }
    return result;
}
    
// Uses
void 
Uses::elaborate(Schema* schema, Node* parent)
{
    QnameBodyStatement::elaborate(schema, parent);
}

void 
Uses::eval(Schema* schema) 
{
    const Common::Qname& qname = getQname();
    Grouping* grouping = _parent->findGrouping(qname.getSuffix());
    if (grouping == 0) {
        YDB_N_ERROR("Failed finding grouping", qname.getSuffix().c_str());
    }
    NamedInteriorNodes& nin = grouping->getInteriorNodes();
    for (NamedInteriorNodes::iterator i = nin.begin(); i != nin.end(); ++i) {
        _parent->addList(i->second);
    }
    Leafs& leafs = grouping->getLeafs();
    for (Leafs::iterator j = leafs.begin(); j != leafs.end(); ++j) {
        _parent->addLeaf(j->second);
    }
}

// Augment
void 
Augment::elaborate(Schema* schema, Node* parent)
{
    Node::elaborate(schema, parent);
    for (Nodes::const_iterator i = _body->begin(); 
         i != _body->end(); 
         ++i) {
        (*i)->elaborate(schema, this);
    }
}

// The target node MUST be either a container, list, choice, case, input,
// output, or notification node.
// If the target node is a container, list, case, input, output, or
// notification node, the "container", "leaf", "list", "leaf-list",
// "uses", and "choice" statements can be used within the "augment"
// statement.
// If the target node is a choice node, the "case" statement, or a case
// shorthand statement (see Section 7.9.2) can be used within the
// "augment" statement.
void 
Augment::eval(Schema* schema) 
{
    for (Nodes::const_iterator i = _body->begin(); 
         i != _body->end(); 
         ++i) {
        (*i)->eval(schema);
    }
    const Common::Qnames& _qnames = _asni->getQnames();
    // grammar ensures size >0
    Common::Qnames::const_iterator i = _qnames.begin();
    // @todo handle prefixes an more target nodes
    const std::string& name = (*i)->getSuffix();
    i++;
    Yang::Ast::InteriorNode* target = schema->findInteriorNode(name);
    for (; i != _qnames.end() && target != 0; ++i) {
        const std::string& name = (*i)->getSuffix();
        target = target->findInteriorNode(name);
    }
    if (target == 0) {
        YDB_N_ERROR("Could not find augument target", getTypeName());
    }
    // add leafs, leaf-lists
    for (Leafs::iterator j = _leafs.begin(); j != _leafs.end();++j) {
        target->addLeaf(j->second);
    }
    // add lists, containers
    for (NamedInteriorNodes::iterator k = _interiorNodes.begin();
         k != _interiorNodes.end();
         ++k){
        target->addList(k->second);
    }
}

void 
Augment::addLeaf(LeafBase* leaf)
{
    const std::string& name = leaf->getString();
    Leafs::iterator i = _leafs.find(name);
    if (i != _leafs.end()) {
        YDB_N_ERROR("Duplicate leafs", getTypeName());
    }
    _leafs[name] = leaf;
}

void 
Augment::addList(InteriorNode* list)
{
    const std::string& name = list->getString();
    NamedInteriorNodes::iterator i = _interiorNodes.find(name);
    if (i != _interiorNodes.end()) {
        YDB_N_ERROR("Duplicate lists", getTypeName());
    }
    _interiorNodes[name] =  list;
}

// Default
void 
Default::elaborate(Schema* schema, Node* parent)
{
    StringStatement::elaborate(schema, parent);
    parent->addDefault(getString());
}

// Container
void 
Container::elaborate(Schema* schema, Node* parent)
{
    StringBodyStatement::elaborate(schema, parent);
    parent->addList(this);
}
void 
Container::addGrouping(Grouping* grouping)
{
    const std::string& name = grouping->getString();
    Groupings::iterator i = _groupings.find(name);
    if (i != _groupings.end()) {
        YDB_N_ERROR("duplicate groupings definiton", name.c_str());
    }
    _groupings[name] =  grouping;
}
 
Grouping* 
Container::findGrouping(const std::string& name)
{
    Grouping* result = 0;
    Groupings::iterator i = _groupings.find(name);
    if (i != _groupings.end()) {
        result = i->second;
    }
    return (result == 0 ? _parent->findGrouping(name) : result);
}

YDB::Node*
Container::create(YDB::Node* parent, DomUtils& domUtils, DOMNode* dn) const
{
    return new YDB::Container(this, domUtils, parent, dn);
}
    
// LeafBase
void 
LeafBase::elaborate(Schema* schema, Node* parent)
{
    StringBodyStatement::elaborate(schema, parent);
    parent->addLeaf(this);
}

bool 
LeafBase::validCardinality(unsigned int i) const
{
    bool result = true;
    unsigned int* min = _minElements.get();
    if (min != 0) {
        result = i >= *min;
    }
    if (result) {
        unsigned int* max = _maxElements.get();
        if (max != 0) {
            result = i <= *max;
        }
    }
    return result;
}

void 
LeafBase::addType(Type* type)
{
    if (_type.get() != 0) {
        YDB_ERROR("Multiple type statements.");
    }
    _type.reset(type);
}

// Leaf
void 
Leaf::eval(Schema* schema)
{
    LeafBase::eval(schema);
    bool mandatory(false);
    if (_mandatory.get() != 0) {
        mandatory = *_mandatory;
    }
    if (mandatory) {
        _minElements.reset(new unsigned int(1));
        _maxElements.reset(new unsigned int(1));
    } else {
        _minElements.reset(new unsigned int(0));
        _maxElements.reset(new unsigned int(1));
    }
    if (mandatory && _defaults.size() == 1) {
        YDB_N_ERROR("leaf is mandatory and has default", getString().c_str());
    }
}

void 
Leaf::setMandatory(bool b)
{
    if (_mandatory.get() != 0) {
        YDB_ERROR("Multiple mandatory statememts");
    }
    _mandatory.reset(new bool(b));
}

void 
Leaf::addDefault(const std::string& d)
{
    if (_defaults.size() == 1) {
        YDB_N_ERROR("leaf can only have one default statment", 
                    getString().c_str());
    }
    _defaults.push_back(d);
}

YDB::Node*
Leaf::create(YDB::Node* parent, DomUtils& domUtils, xercesc::DOMNode* dn) const
{
    return new YDB::Leaf(this, domUtils, parent, dn);
}
    
// LeafList
void 
LeafList::eval(Schema* schema)
{
    LeafBase::eval(schema);
    unsigned int defaultSize = _defaults.size();
    if (defaultSize != 0) {
        if (getMinElements() != 0) {
            YDB_N_ERROR("leaf-list can not have default value if minElements > 0", 
                        getString().c_str());
        }
        if (!validCardinality(defaultSize)) {
            YDB_N_ERROR("leaf-list's number of defaults has wrong cardinality", 
                        getString().c_str());
        }
    }
}

// List
void 
List::elaborate(Schema* schema, Node* parent)
{
    StringBodyStatement::elaborate(schema, parent);
    parent->addList(this);
}

void 
List::eval(Schema* schema)
{
    StringBodyStatement::eval(schema);
    if (_key.get() == 0) {
        YDB_N_ERROR("Must have a key statment.", _s.get()->c_str());
    }
}

void 
List::addKey(Key* key)
{
    if (_key.get() != 0) {
        YDB_N_ERROR("Muliple key statements", _s.get()->c_str());
    }
    _key.reset(key);
}

const DbVal*
List::createKey(const Common::KeyVals& leafs) const {
    std::vector<const DbVal*> vals;
    const Common::Qnames& keys = _key->getKeys();
    for (Common::Qnames::const_iterator i = keys.begin(); 
         i != keys.end();
         ++i) {
        const std::string& key = (*i)->getString();
        size_t keyCount = leafs.count(key);
        if (keyCount != 1) {
            std::stringstream ss;
            ss << "Wrong cardinality for key: " << key  
               << ", count: " << keyCount << ", list: " << getString();
            YDB_ERROR(ss.str());
        }
        Common::KeyVals::const_iterator j = leafs.find(key);
        Leafs::const_iterator k = _leafs.find(key);
        if (k == _leafs.end()) {
            YDB_ERROR("Could not find key leaf, inconsitent model");
        }
        const LeafBase* leaf = k->second;
        const Type& type = leaf->getType();
        vals.push_back(type.create(j->second));
    } 
    const DbVal* result;
    switch (vals.size()) {
    case 0: 
        //  @todo memory leak
        Common::Error::err(__PRETTY_FUNCTION__, "Could not find key");
        break;
    case 1:
        result = vals.front();
        break;
    default:
        //@todo implement
        Common::Error::err(__PRETTY_FUNCTION__, "Composite keys not implemented");
    }
    return result;
}

void 
List::addGrouping(Grouping* grouping)
{
    const std::string& name = grouping->getString();
    Groupings::iterator i = _groupings.find(name);
    if (i != _groupings.end()) {
        YDB_N_ERROR("duplicate groupingsdefiniton", name.c_str());
    }
    _groupings[name] =  grouping;
}
 
Grouping* 
List::findGrouping(const std::string& name)
{
    Grouping* result = 0;
    Groupings::iterator i = _groupings.find(name);
    if (i != _groupings.end()) {
        result = i->second;
    }
    return (result == 0 ? _parent->findGrouping(name) : result);
}

}

}
