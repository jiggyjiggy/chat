#include "MessageCommand.h"
#include "SessionManager.h"
#include "Session.h"
#include <iostream>
MessageCommand::MessageCommand(SessionManager& sessionManager)
    : mSessionManager(sessionManager)
{
}

void MessageCommand::execute(std::vector<std::string>& args, Session& session)
{
    std::string message = args[0];
	mSessionManager.sendAll("[" + session.getUsername() + "] " + message);
}
