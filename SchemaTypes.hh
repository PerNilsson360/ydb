#ifndef _SCHEMA_TYPES_HH_
#define _SCHEMA_TYPES_HH_

#include "Common.hh"
#include "DbVal.hh"

/*
       +---------------------+-------------------------------------+
       | Name                | Description                         |
       +---------------------+-------------------------------------+
       | binary              | Any binary data                     |
       | bits                | A set of bits or flags              |
       | boolean             | "true" or "false"                   |
       | decimal64           | 64-bit signed decimal number        |
       | empty               | A leaf that does not have any value |
       | enumeration         | Enumerated strings                  |
       | identityref         | A reference to an abstract identity |
       | instance-identifier | References a data tree node         |
       | int8                | 8-bit signed integer                |
       | int16               | 16-bit signed integer               |
       | int32               | 32-bit signed integer               |
       | int64               | 64-bit signed integer               |
       | leafref             | A reference to a leaf instance      |
       | string              | Human-readable string               |
       | uint8               | 8-bit unsigned integer              |
       | uint16              | 16-bit unsigned integer             |
       | uint32              | 32-bit unsigned integer             |
       | uint64              | 64-bit unsigned integer             |
       | union               | Choice of member types              |
       +---------------------+-------------------------------------+

 */
class SchemaType
{
public:
    virtual const DbVal* create(const std::string& data) {
        Common::Error::err(__PRETTY_FUNCTION__, "Wrong type.");
    }
private:
};

class SchemaBinary : public SchemaType
{
};

class SchemaBits : public SchemaType
{
};

class SchemaBoolean : public SchemaType
{
};

class SchemaDecimal64 : public SchemaType
{
};

class SchemaEmpty : public SchemaType
{
};

class SchemaEnumeration : public SchemaType
{
};

class SchemaIdentityref : public SchemaType
{
};

class SchemaInstanceIdentifier : public SchemaType
{
};

class SchemaInt8 : public SchemaType
{
};

class SchemaInt16 : public SchemaType
{
};

class SchemaInt32 : public SchemaType
{
};

class SchemaInt64 : public SchemaType
{
};

class SchemaLeafref : public SchemaType
{
};

class SchemaString : public SchemaType
{
public:
    const DbVal* create(const std::string& data) { return new String(data); }
};

class SchemaUInt8 : public SchemaType
{
};

class SchemaUInt16 : public SchemaType
{
};

class SchemaUInt32 : public SchemaType
{
};

class SchemaUInt64 : public SchemaType
{
};

class SchemaUnion : public SchemaType
{
};

class DerivedString : public SchemaString
{
};

class DerivedInt64 : public SchemaInt64
{
};

#endif
