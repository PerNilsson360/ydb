#ifndef _NODE_HH_
#define _NODE_HH_

#include <map>

#include <xercesc/dom/DOM.hpp>

#include "Key.hh"

using namespace xercesc;

namespace Yang
{
namespace Ast
{
    class Container;
    class Leaf;
}
}

class DomUtils;

namespace YDB
{

class Node;
typedef std::map<YDB::Key, Node*> Nodes;
    
class Node
{
public:
    Node(DomUtils& domUtils, Node* parent, DOMNode* domNode);
    virtual ~Node();
    // Will take over the memory owned by node
    virtual void merge(Node* node) = 0;
    virtual const YDB::Key getKey() const = 0;
    DOMNode* getDomNode() { return _domNode; }
    Nodes& getNodes() { return _nodes; }
    virtual void setText(const XMLCh* text) { /* default a nop*/ }
    // Disassociate its self with its parent and
    // gives the dom node away.
    DOMNode* release();
    void removeChild(Node* node);
protected:
    DomUtils& _domUtils;
    Node* _parent; 
    DOMNode* _domNode;
    Nodes _nodes;
};

class LeafNode;
    
class InteriorNode : public Node
{
public:
    InteriorNode(DomUtils& domUtils,
		 Node* parent,
		 DOMNode* domNode);
private:
};

class Container : public InteriorNode
{
public:
    Container(const Yang::Ast::Container* schema,
	      DomUtils& domUtils,
	      Node* parent,
	      DOMNode* domNode);
    void merge(Node* node);
    const YDB::Key getKey() const;
private:
    const Yang::Ast::Container* _schema;
};

class List : public InteriorNode
{
};

class LeafNode : public Node
{
public:
    LeafNode(DomUtils& domUtils, Node* parent, DOMNode* domNode);
private:
};

class Leaf : public LeafNode
{
public:
    Leaf(const Yang::Ast::Leaf* schema,
	 DomUtils& domUtils,
	 Node* parent,
	 DOMNode* domNode);
    void merge(Node* node);
    const YDB::Key getKey() const;
    virtual void setText(const XMLCh* text);
private:
    const Yang::Ast::Leaf* _schema;
};

class LeafList : public LeafNode
{
};

}

#endif
