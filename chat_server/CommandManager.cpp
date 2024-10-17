#include "CommandManager.h"
#include "command/ICommandable.h"
#include "command/ChangeCommand.h"
#include "command/DefaultCommand.h"
#include "command/ExitCommand.h"
#include "command/JoinCommand.h"
#include "command/MessageCommand.h"
#include "command/UsersCommand.h"
#include "SessionManager.h"
#include <sstream>
#include <iostream>

CommandManager::CommandManager(SessionManager& sessionManager)
{
    mCommands["/join"] = new JoinCommand(sessionManager);
    mCommands["/message"] = new MessageCommand(sessionManager);
    mCommands["/change"] = new ChangeCommand(sessionManager);
    mCommands["/users"] = new UsersCommand(sessionManager);
    mCommands["/exit"] = new ExitCommand();

    mDefaultCommand = new DefaultCommand();
}

CommandManager::~CommandManager()
{
    for (auto& commandPair : mCommands) {
        delete commandPair.second;
    }

    delete mDefaultCommand;
}

void CommandManager::execute(const std::string& totalMessage, Session& session)
{
	std::cout << "CommandManager::execute() 시작" << std::endl;

	const char DELIMITER = '|';
    std::istringstream iss(totalMessage);
    std::string key;
    std::getline(iss, key, DELIMITER);
	std::cout << "key : " << key << std::endl;

    // 명령어를 찾아 실행하거나 기본 명령어 실행
    auto commandIter = mCommands.find(key);
    ICommandable* command = (commandIter != mCommands.end()) 
                            ? commandIter->second 
                            : mDefaultCommand;

    std::vector<std::string> args;
    std::string arg;
    while (std::getline(iss, arg, DELIMITER)) 
	{
        args.push_back(arg);
    }

    command->execute(args, session);

	std::cout << "CommandManager::execute() 끝" << std::endl;
}
