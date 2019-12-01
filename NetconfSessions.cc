#include <sstream>

#include "NetconfException.hh"
#include "NetconfSessions.hh"

NetconfSessions::NetconfSessions() :
    _sessions(MAX_SESSIONS, NetconfSession())
{
}

void
NetconfSessions::newSession(int socket)
{
    int i = 0;
    for (; i < MAX_SESSIONS; i++) {
	if (_sessions[i].getSocket() == 0) {
	    _sessions[i] = NetconfSession(_sessionId++, socket);
	    break;
	}
    }
    if (i == MAX_SESSIONS) {
	std::stringstream ss;
	ss << "Max number of sessions has been reached";
	throw ResourceDenied(ss, ResourceDenied::Application);
    }
}
