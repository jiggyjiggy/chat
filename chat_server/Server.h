#ifndef SERVER_H
#define SERVER_H

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
	int mServerSocketFd;

    void running();
};

#endif /* SERVER_H */