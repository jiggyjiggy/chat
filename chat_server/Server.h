#ifndef SERVER_H
#define SERVER_H

#include "ThreadPoolManager.h"

class CommandManager;
class SessionManager;

class Server
{
public:
	Server(int port, CommandManager& commandManager, SessionManager& sessionManager);
	void start();

private:
	int mPort;
	CommandManager& mCommandManager;
	SessionManager& mSessionManager;

	ThreadPoolManager mPool; // 멤버로 ThreadPoolManager 정의

	int mServerSocketFd;

    void running();
};

#endif /* SERVER_H */