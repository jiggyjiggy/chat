#include "Server.h"
#include "SessionManager.h"
#include "CommandManager.h"

int main()
{
	const int PORT = 12345;

	SessionManager sessionManager;
	CommandManager commandManager(sessionManager);
	Server server(PORT, commandManager, sessionManager);
	server.start();

	return 0;
}