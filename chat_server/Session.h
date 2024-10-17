#ifndef SESSION_H
#define SESSION_H

#include <string>

class CommandManager;
class SessionManager;

class Session
{
public:
	Session(int socketFd, CommandManager& commandManager, SessionManager& sessionManager);
	~Session();

	void run();
	void send(const std::string& message);
	void close();

	std::string& getUsername();
	void setUsername(std::string& username);

	bool operator==(const Session& other) const;
	
private:
	int mSocketFd;
	CommandManager* mCommandManager;
	SessionManager* mSessionManager;
	bool mbClosed;
	std::string mUsername;
};

#endif /* SESSION_H */