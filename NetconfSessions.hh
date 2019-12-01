#ifndef _NETCONF_SESSIONS_HH_
#define _NETCONF_SESSIONS_HH_

#include <vector>

#include "NetconfSession.hh"

class NetconfSessions
{
public:
    static const int MAX_SESSIONS = 30;
    NetconfSessions();
    std::vector<NetconfSession>& getSessions() {
	return _sessions;
    }
    void newSession(int socket);
private:
    uint16_t _sessionId = 0;
    std::vector<NetconfSession> _sessions;
};

#endif
