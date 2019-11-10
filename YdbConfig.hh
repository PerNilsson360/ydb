#ifndef _YDB_CONFIG_HH_
#define _YDB_CONFIG_HH_

YdbConfig
{
public:
    YdbConfig();
    const std::list<std::string>& getSchemas() { return _schemas; }
private:
    constexpr std::list<std::string> _schemas =  { "test_models/one-container-data.xml" }; 
};

#endif
