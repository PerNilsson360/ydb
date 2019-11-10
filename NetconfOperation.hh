#ifndef _NETCONF_OPERATION_HH_
#define _NETCONF_OPERATION_HH_

#include <xercesc/dom/DOM.hpp>

#include "Ydb.hh"

class NetconfResponse;

class NetconfOperation
{
public:
    NetconfOperation(xercesc::DOMNode* rpc);
    static NetconfOperation* create(xercesc::DOMNode* rpc);
    virtual xercesc::DOMNode* apply(Ydb& ydb) = 0;
protected:
    xercesc::DOMNode* _rpc;
private:
    static constexpr const char* GET = "get";
    static constexpr const char* GET_CONFIG = "get-config";
    static constexpr const char* EDIT_CONFIG = "edit-config";
    static constexpr const char* DELETE_CONFIG = "delete-config";
    static constexpr const char* COPY_CONFIG = "copy-config";
    static constexpr const char* LOCK = "lock";
    static constexpr const char* UNLOCK = "unlock";
    static constexpr const char* CLOSE_SESSION = "close-session";
    static constexpr const char* KILL_SESSION = "kill-session";
};

class Get : public NetconfOperation
{
public:
    Get(xercesc::DOMNode* rpc);
    xercesc::DOMNode* apply(Ydb& ydb);
};

class GetConfig : public NetconfOperation
{
public:
    GetConfig(xercesc::DOMNode* rpc);
    xercesc::DOMNode* apply(Ydb& ydb);
};

class EditConfig : public NetconfOperation
{
public:
    EditConfig(xercesc::DOMNode* rpc);
    xercesc::DOMNode* apply(Ydb& ydb);
private:
    Ydb::ErrorOption getErrorOption(xercesc::DOMNode* errorOption);
};

class CopyConfig : public NetconfOperation
{
public:
    CopyConfig(xercesc::DOMNode* rpc);
    xercesc::DOMNode* apply(Ydb& ydb);
};

class DeleteConfig : public NetconfOperation
{
public:
    DeleteConfig(xercesc::DOMNode* node);
    xercesc::DOMNode* apply(Ydb& ydb);
};

class Lock : public NetconfOperation
{
public:
    Lock(xercesc::DOMNode* rpc);
    xercesc::DOMNode* apply(Ydb& ydb);
};

class Unlock : public NetconfOperation
{
public:
    Unlock(xercesc::DOMNode* rpc);
    xercesc::DOMNode* apply(Ydb& ydb);
};

class CloseSession : public NetconfOperation
{
public:
    CloseSession(xercesc::DOMNode* rpc);
    xercesc::DOMNode* apply(Ydb& ydb);
};

class KillSession : public NetconfOperation
{
public:
    KillSession(xercesc::DOMNode* rpc);
    xercesc::DOMNode* apply(Ydb& ydb);
};

#endif
