#include "Matterbot.h"
#include "config.h"
#include "main.h"
#include <iostream>
#include <sstream>
#include "PracticalSocket.h"
#include "ConsoleLogger.h"
#include "EchoCommand.h"
#include "ReverseCommand.h"
#include "QotdCommand.h"
#include "WeatherCommand.h"
#include "LevelCommand.h"
#include "StatusCommand.h"
#include "TimerCommand.h"
#include "NumberCommand.h"
#include "GetPassword.h"
#include "StartCommand.h"
#include "RestartCommand.h"
#include "HRestartCommand.h"
#include "ChallengesCommand.h"
#include "ScrapsCommand.h"

#define ERROR_SUCCESS 0
#define ERROR_FAILURE -1


using namespace std;
using namespace lospi;

int main() {
  wstring welcome_message = L"bot is running.";

  try {
    auto bot = make_shared<Matterbot>(MATTERMOST_URL, INC_HOOK_ROUTE, OUT_HOOK_ROUTE, OUT_HOOK_TOKEN);
    bot->set_logger(make_unique<ConsoleLogger>());
	bot->register_command(make_shared<EchoCommand>());
	bot->register_command(make_shared<ReverseCommand>(bot));
	bot->register_command(make_shared<QotdCommand>(bot));
	bot->register_command(make_shared<WeatherCommand>(bot));
	bot->register_command(make_shared<LevelCommand>(bot));
	bot->register_command(make_shared<TimerCommand>(bot));
	bot->register_command(make_shared<StartCommand>(bot)); 
	bot->register_command(make_shared<StatusCommand>(bot));
	bot->register_command(make_shared<RestartCommand>(bot));
	bot->register_command(make_shared<NumberCommand>(bot));
	bot->register_command(make_shared<PasswordCommand>(bot)); 
	bot->register_command(make_shared<HardRestartCommand>(bot));
	auto chl_ptr = make_shared<ChallengesCommand>(bot);
	bot->register_command(chl_ptr);
	bot->register_command(make_shared<ScrapsCommand>(bot, chl_ptr));
    bot->post_message(welcome_message);

    wstring console;
    wclog << ">> Type \'quit\' to quit. Any other input will be sent to the bot." << endl;
    while (getline(wcin, console)) {
      if (L"quit" == console) {
        wclog << ">> Quitting." << endl;
        return ERROR_SUCCESS;
      }
      if (L"" != console) { bot->post_message(console); }
    }
  }
  catch (exception e) {
    wcerr << "[-] Unhandled exception:" << e.what() << endl;
    return ERROR_FAILURE;
  }
}

std::vector<std::string> explode(std::string const & s, char delim)
{
	std::vector<std::string> result;
	std::istringstream iss(s);

	for (std::string token; std::getline(iss, token, delim); )
	{
		result.push_back(std::move(token));
	}

	return result;
}