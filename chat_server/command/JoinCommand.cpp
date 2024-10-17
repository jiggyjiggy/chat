#include "JoinCommand.h"
#include "Session.h"
#include "SessionManager.h"
#include <vector>
#include <string>
#include <iostream>

JoinCommand::JoinCommand(SessionManager& sessionManager)
    : mSessionManager(sessionManager)
{
}

void JoinCommand::execute(std::vector<std::string>& args, Session& session) 
{
	std::cout << "JoinCommand::execute" << std::endl;
	std::string username = args[0];
	session.setUsername(username);
	mSessionManager.sendAll(username + "님이 입장했습니다.");
}
