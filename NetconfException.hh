#ifndef _NETCONFEXCEPTION_HH_
#define _NETCONFEXCEPTION_HH_

#include <string>
#include <sstream>
#include <stdexcept>

#include <xercesc/dom/DOM.hpp>

class DomUtils;

class NetconfException : public std::runtime_error
{  
public:
    NetconfException(const std::stringstream& message) :
	std::runtime_error(message.str()) {}
    virtual std::string createResponse(DomUtils& domUtils,
				       xercesc::DOMNode* rpc) const = 0;
protected:
    xercesc::DOMNode* createRpcReply(xercesc::DOMDocument* document,
				     xercesc::DOMNode* rpc) const;
};

class BadElement : public NetconfException
{
public:
    enum ErrorType {
	Protocol,
	Application
    };
    BadElement(const std::stringstream& message,
	       ErrorType errorType) :
	NetconfException(message),
	_errorType(errorType) {}
    std::string createResponse(DomUtils& domUtils,
			       xercesc::DOMNode* rpc) const override;
private:
    ErrorType _errorType;
};

class OperationNotSupported : public NetconfException
{
public:
    enum ErrorType {
	Protocol,
	Application
    };
    OperationNotSupported(const std::stringstream& message,
			  ErrorType errorType) :
	NetconfException(message),
	_errorType(errorType) {}
    std::string createResponse(DomUtils& domUtils,
			       xercesc::DOMNode* rpc) const override;
private:
    ErrorType _errorType;
};

class OperationFailed : public NetconfException
{
public:
    enum ErrorType {
	Rpc,
	Protocol,
	Application
    };
    OperationFailed(const std::stringstream& message, ErrorType errorType) :
	NetconfException(message) {}
    std::string createResponse(DomUtils& domUtils) const;
    std::string createResponse(DomUtils& domUtils,
			       xercesc::DOMNode* rpc) const override;
private:
    ErrorType _errorType;
};

class ResourceDenied : public NetconfException
{
public:
    enum ErrorType {
	Rpc,
	Protocol,
	Application
    };
    ResourceDenied(const std::stringstream& message, ErrorType errorType) :
	NetconfException(message) {}
    std::string createResponse(DomUtils& domUtils) const;
    std::string createResponse(DomUtils& domUtils,
			       xercesc::DOMNode* rpc) const override;
private:
    ErrorType _errorType;
};

#endif
