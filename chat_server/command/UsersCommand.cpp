#include "UsersCommand.h"
#include "SessionManager.h"
#include "Session.h"
#include <sstream> 
#include <stdexcept>

UsersCommand::UsersCommand(SessionManager& sessionManager)
    : mSessionManager(sessionManager)
{
}

void UsersCommand::execute(std::vector<std::string>& args, Session& session)
{
    std::vector<std::string> usernames = mSessionManager.getAllUsername();

    std::ostringstream oss;
    oss << "전체 접속자 : " << usernames.size() << "\n";
    for (const auto& username : usernames) {
        oss << " - " << username << "\n";
    }

    session.send(oss.str());
}
