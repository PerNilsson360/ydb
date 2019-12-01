#ifndef _CAPABILTYEXCHANGE_HH_
#define _CAPABILTYEXCHANGE_HH_

class CapabilityExchange
{
public:
    const char* getMessage() { return _message; }
private:
    const char* _message =
 "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
 "<hello>\n"
   "<capabilities>\n"
     "<capability>\n"
       "urn:ietf:params:xml:ns:netconf:base:1.0\n"
     "</capability>\n"
     "<capability>\n"
       "urn:ietf:params:ns:netconf:capability:startup:1.0\n"
     "</capability>\n"
   "</capabilities>\n"
   "<session-id>4</session-id>\n"
 "</hello>\n"
 "]]>]]>";
};

#endif
