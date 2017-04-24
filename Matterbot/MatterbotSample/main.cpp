#include "Matterbot.h"
#include <iostream>
#include <sstream>
#include "ConsoleLogger.h"
#include "EchoCommand.h"
#include "ReverseCommand.h"
//#include "QotdCommand.h"
//#include "WeatherCommand.h"
#include "LevelCommand.h"
#include "StartCommand.h"
#include "NumberCommand.h"
#include "GetPassword.h"
#include "RestartCommand.h"
#include "ChallengesCommand.h"

#define ERROR_SUCCESS 0
#define ERROR_FAILURE -1


using namespace std;
using namespace lospi;

int main() {
  wstring mattermost_url = L"https://hooks.slack.com/",
    incoming_hook_route = L"services/T4Z1QAKSS/B50MWS6CX/YJmejxyLjz50lvPrk4FUrYUe",
    outgoing_hook_route = L"http://127.0.0.1:4928/",
    outgoing_hook_token = L"3q3YsAQD376YdVsU5uTjWKh5",
    welcome_message = L"bot is running.";

  try {
    auto bot = make_shared<Matterbot>(mattermost_url, incoming_hook_route, outgoing_hook_route, outgoing_hook_token);
    bot->set_logger(make_unique<ConsoleLogger>());
	bot->register_command(make_shared<EchoCommand>());
	bot->register_command(make_shared<ReverseCommand>(bot));
	//bot->register_command(make_shared<QotdCommand>(bot));
	//bot->register_command(make_shared<WeatherCommand>(bot));
	bot->register_command(make_shared<LevelCommand>(bot));
	bot->register_command(make_shared<StartCommand>(bot)); 
	bot->register_command(make_shared<StatusCommand>(bot));
	bot->register_command(make_shared<RestartCommand>(bot));
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