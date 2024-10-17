#include "ChangeCommand.h"
#include "SessionManager.h"
#include "Session.h"

ChangeCommand::ChangeCommand(SessionManager& sessionManager)
	: mSessionManager(sessionManager)
{
}

void ChangeCommand::execute(std::vector<std::string>& args, Session& session)
{
	std::string changeName = args[0];
	mSessionManager.sendAll(session.getUsername() + "님이 " + changeName + "로 이름을 변경했습니다.");
	session.setUsername(changeName);
}
