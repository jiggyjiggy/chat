#ifndef SESSION_MANAGER_H
#define SESSION_MANAGER_H

#include <vector>
#include <string>
#include <mutex>
#include "Session.h"

class Session;

class SessionManager
{
public:	

	void add(const Session& session);
	void remove(const Session& session);
	void closeAll();
	void sendAll(const std::string& message) const;
	std::vector<std::string> getAllUsername() const;

private:
	std::vector<Session> mSessions;
	mutable std::mutex mMutex;
};

#endif /* SESSION_MANAGER_H */