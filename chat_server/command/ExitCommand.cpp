#include "ExitCommand.h"
#include "Session.h"
#include <stdexcept>

void ExitCommand::execute(std::vector<std::string>& args, Session& session)
{
    throw std::runtime_error("exit");
}
