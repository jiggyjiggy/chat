#ifndef EXIT_COMMAND_H
#define EXIT_COMMAND_H

#include "ICommandable.h"

class ExitCommand : public ICommandable
{
	void execute(std::vector<std::string>& args, Session& session);
};

#endif /* EXIT_COMMAND_H */