#ifndef CHANGE_COMMAND_H
#define CHANGE_COMMAND_H

#include "ICommandable.h"

class SessionManager;
class Session;

class ChangeCommand : public ICommandable
{
public:
	ChangeCommand(SessionManager& sessionManager);

	void execute(std::vector<std::string>& args, Session& session);

private:
	SessionManager& mSessionManager;
};

#endif /* CHANGE_COMMAND_H */