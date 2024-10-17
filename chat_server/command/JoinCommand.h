#ifndef JOIN_COMMAND_H
#define JOIN_COMMAND_H

#include "ICommandable.h"
#include <vector>
#include <string>

class SessionManager;
class Session;

class JoinCommand : public ICommandable
{
public:
	JoinCommand(SessionManager& sessionManager);

	void execute(std::vector<std::string>& args, Session& session);

private:
	SessionManager& mSessionManager;
};

#endif /* JOIN_COMMAND_H */