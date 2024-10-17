#ifndef USERS_COMMAND_H
#define USERS_COMMAND_H

#include "ICommandable.h"
#include <vector>
#include <string>

class SessionManager;
class Session;

class UsersCommand : public ICommandable
{
public:
	UsersCommand(SessionManager& sessionManager);
	
	void execute(std::vector<std::string>& args, Session& session);

private:
	SessionManager& mSessionManager;
};

#endif /* USERS_COMMAND_H */