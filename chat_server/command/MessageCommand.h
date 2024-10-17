#ifndef MESSAGE_COMMAND_H
#define MESSAGE_COMMAND_H

#include "ICommandable.h"
#include <vector>
#include <string>

class SessionManager;
class Session;

class MessageCommand : public ICommandable
{
public:
	MessageCommand(SessionManager& sessionManager);
	void execute(std::vector<std::string>& args, Session& session);

private:
	SessionManager& mSessionManager;
};

#endif /* MESSAGE_COMMAND_H */