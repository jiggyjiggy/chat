#ifndef DEFAULT_COMMAND_H
#define DEFAULT_COMMAND_H

#include "ICommandable.h"

class Session;

class DefaultCommand : public ICommandable
{
public:
	void execute(std::vector<std::string>& args, Session& session);
};

#endif /* DEFAULT_COMMAND_H */