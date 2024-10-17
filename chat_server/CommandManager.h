#include <string>
#include <unordered_map>

class SessionManager;
class Session;
class ICommandable;

class CommandManager
{
public:

	CommandManager(SessionManager& sessionManager);
	~CommandManager();

	void execute(const std::string& totalMessage, Session& session);

private:
	std::unordered_map<std::string, ICommandable*> mCommands;
	ICommandable* mDefaultCommand;
};