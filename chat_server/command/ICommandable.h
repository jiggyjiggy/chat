#ifndef I_COMMANDABLE_H
#define I_COMMANDABLE_H

#include <vector>
#include <string>

class Session;

class ICommandable
{
public:
	virtual ~ICommandable() {};
	
	virtual void execute(std::vector<std::string>& args, Session& session) = 0;
};

#endif /* I_COMMANDABLE_H */