#ifndef _CONFIG_HH_
#define _CONFIG_HH_

#include <list>
#include <string>

#include "YdbStr.hh"

class DomUtils;

class Config
{
public:
    Config(DomUtils& domUtils);
    const std::list<std::string>& getSchemas() const { return _schemas; }
    const std::string& getRunningDbFile() const { return _runningDbFile; }
private:
    static const YdbStr YDB_CONFIG;
    static const YdbStr SCHEMA_PATH;
    static const YdbStr SCHEMA;
    void read();
    DomUtils& _domUtils;
    const char*  _ydbConfigEnvVar = "YDB_CONFIG_FILE";
    std::string _ydbConfigFile = "testModels/ydb-config.xml";
    std::string _runningDbFile = "/tmp/ydb-running.db";
    std::string _schemaPath;
    std::list<std::string> _schemas =  { "test_models/one-container.yang" }; 
};

#endif
