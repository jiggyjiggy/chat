#include "SessionManager.h"
#include "Session.h"
#include <algorithm>
#include <iostream>
#include <unistd.h>

void SessionManager::add(Session* session)
{
	std::lock_guard<std::mutex> lock(mMutex);

	mSessions.push_back(session);
}

void SessionManager::remove(const Session& session) {
    std::lock_guard<std::mutex> lock(mMutex);

    auto it = std::remove_if(mSessions.begin(), mSessions.end(), [&session](const Session* s) { return *s == session; });

    mSessions.erase(it, mSessions.end());
}

void SessionManager::closeAll()
{
	std::lock_guard<std::mutex> lock(mMutex);
	for (Session* session : mSessions)
	{
		session->close();
	}
	mSessions.clear();
}

void SessionManager::sendAll(const std::string& message) const
{
	std::lock_guard<std::mutex> lock(mMutex);

	std::cout << "sendAll() 시작" << std::endl;
	for (Session* session : mSessions)
	{
		session->send(message);
	}
}

std::vector<std::string> SessionManager::getAllUsername() const
{
    std::lock_guard<std::mutex> lock(mMutex);

    std::vector<std::string> usernames;
    for (Session* session : mSessions)
	{
        if (session->getUsername().empty() == false) 
		{
            usernames.push_back(session->getUsername());
        }
    }
    return usernames;
}

