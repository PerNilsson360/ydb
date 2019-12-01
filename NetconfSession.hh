#ifndef _NETCONF_SESSION_HH_
#define _NETCONF_SESSION_HH_

class NetconfSession
{
public:
    NetconfSession() {}
    NetconfSession(uint16_t sessionId, int socket) :
	_sessionId(sessionId),
	_socket(socket) {}
    void reset() { _socket = 0; }
    int getSocket() const { return _socket; }
private:
    uint16_t _sessionId = 0;
    int _socket = 0;		// indicates not initialized
};

#endif
