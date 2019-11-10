#include "Node.hh"

#include "YangAst.hh"
#include "YdbStr.hh"
#include "NetconfException.hh"

namespace YDB
{

// Node
Node::Node(DomUtils& domUtils, Node* parent, DOMNode* domNode) :
    _domUtils(domUtils),
    _parent(parent),
    _domNode(domNode)
{
}

Node::~Node()
{
}
    
DOMNode*
Node::release()
{
    std::cerr << "release" << std::endl;
    if (_parent != nullptr) {
	_parent->removeChild(this);
    }
    DOMNode* result = _domNode;
    _domNode = nullptr;
    return result;
}

void
Node::removeChild(Node* node)
{
    Key key = node->getKey();
    Nodes::iterator i = _nodes.find(key);
    if (i != _nodes.end()) {
	_nodes.erase(i);
    }
}

// InteriorNode
InteriorNode::InteriorNode(DomUtils& domUtils,
			   Node* parent,
			   DOMNode* domNode) :
    Node(domUtils, parent, domNode)
{
}

// Container
Container::Container(const Yang::Ast::Container* schema,
		     DomUtils& domUtils,
		     Node* parent,
		     DOMNode* domNode) :
    InteriorNode(domUtils, parent, domNode),
    _schema(schema)
{
    YdbStr containerName(domNode->getLocalName());
    DOMNodeList* nodeList = domNode->getChildNodes();
    for (int i = 0; i < nodeList->getLength(); i++) {
	std::cerr << "iu : " << i << " length: "<< nodeList->getLength() << std::endl;
	DOMNode* n = nodeList->item(i);
	if (n->getNodeType() != DOMNode::ELEMENT_NODE) {
	    continue;
	}
	YdbStr name(n->getLocalName());
	std::cout << "child name " << name << std::endl;
	const Yang::Ast::Node* schemaNode = _schema->findNode(name);
	if (schemaNode == nullptr) {
	    std::stringstream ss;
	    ss << "Could not find: " << name << " in schema";
	    throw BadElement(ss, BadElement::Application);
	}
	std::cout << "schema node: " << schemaNode->getTypeName()
		  << std::endl;
	YDB::Node* node = schemaNode->create(this, _domUtils, n);
	YDB::Key key = node->getKey();
	Nodes::iterator j = _nodes.find(key);
	if (j != _nodes.end()) {
	    j->second->merge(node);
	} else {
	    _nodes[key] = node;
	}
    }
}

const YDB::Key
Container::getKey() const
{
    return Key(_schema->getString());
}

void
Container::merge(Node* node)
{
    std::cout << "container merge: "<< std::endl;
    Nodes& mergeNodes = node->getNodes();
    for (auto const& n : mergeNodes) {
	Nodes::iterator i = _nodes.find(n.first);
	if (i == _nodes.end()) {
	    _nodes[n.first] = i->second;
	} else {
	    i->second->merge(n.second);
	}
    }
    mergeNodes.clear();
    delete node;
    std::cout << "container merge end "<< std::endl;
}
    
// LeafNode
LeafNode::LeafNode(DomUtils& domUtils,
		   Node* parent,
		   DOMNode* domNode) :
    Node(domUtils, parent, domNode)
{
}
    
// Leaf
Leaf::Leaf(const Yang::Ast::Leaf* schema,
	   DomUtils& domUtils,
	   Node* parent,
	   DOMNode* domNode) :
    LeafNode(domUtils, parent, domNode),
    _schema(schema)
{
    std::cout << "leaf: " << (void*) domNode <<std::endl;
    for (DOMNode* n = domNode->getFirstChild();
	 n != 0;
	 n = n->getNextSibling()) {
	DOMNode::NodeType nodeType = n->getNodeType();
	if (nodeType == DOMNode::NodeType::TEXT_NODE) {
	    char* value = XMLString::transcode(n->getNodeValue());
	    std::cout << "leafValue: " << value << std::endl;
	    XMLString::release(&value);
	}
    }
    std::cout << "leaf end: "<< std::endl;
}

const YDB::Key
Leaf::getKey() const
{
    return Key(_schema->getString());
}
    
void
Leaf::merge(Node* node)
{
    std::cout << "LeafMerge" << std::endl;
    // @todo refactor use set/getText
    DOMNode* domNode = node->getDomNode();
    for (DOMNode* n = domNode->getFirstChild();
	 n != 0;
	 n = n->getNextSibling()) {
	DOMNode::NodeType nodeType = n->getNodeType();
	if (nodeType == DOMNode::TEXT_NODE) {
	    const XMLCh* v = n->getNodeValue();
	    YdbStr vs(v);
	    std::cout << (void*) _domNode << " value: " << vs << std::endl;
	    setText(n->getNodeValue());
	}
    }
    delete node;
    std::cout << "LeafMerge end" << std::endl;
}

void
Leaf::setText(const XMLCh* text)
{
    DOMNodeList* nodeList = _domNode->getChildNodes();
    for (int i = 0; i < nodeList->getLength(); i++) {
	DOMNode* n = nodeList->item(i);
	if (n->getNodeType() == DOMNode::NodeType::TEXT_NODE) {
	    n->setNodeValue(text);
	}
    }
}
}
