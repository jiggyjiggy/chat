#include "DefaultCommand.h"
#include "Session.h"
#include <sstream>
#include <vector>
#include <string>

void DefaultCommand::execute(std::vector<std::string>& args, Session& session)
{
	std::ostringstream oss;
    oss << "처리할 수 없는 명령어 입니다: [";
    for (size_t i = 0; i < args.size(); ++i) 
	{
        oss << args[i];
        if (i < args.size() - 1) 
		{
            oss << ", ";  // 마지막 요소 뒤에는 쉼표를 넣지 않음
        }
    }
    oss << "]";

    // 변환된 문자열을 세션으로 전송
    session.send(oss.str());
}

