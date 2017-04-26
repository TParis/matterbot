#pragma once
#include "Matterbot.h"
#include <algorithm>
#include <string>
#include <iostream>
#include <ostream>
#include <vector>
#include <thread>
#include <chrono>
#include <map>
#include "Md5.h"
#include "Md5Utilities.h"
#include "GetPassword.h"
#include <functional> 
#include <cctype>
#include <locale>
#include <stdint.h>
// for convenience

namespace lospi {

	//Does the same thing as the challenge command...
	struct ScrapsCommand : ICommand {
		explicit ScrapsCommand(std::shared_ptr<Matterbot> bot, std::shared_ptr<ChallengesCommand> challenges) 
		{ 
			this->bot = bot;
			this->challenges = challenges;
		};
		std::wstring get_name() override { return L"scraps"; }


		std::wstring get_help() override { return L"`scraps`: `scraps` loads the hashes and begins crunching."; }

		std::wstring handle_command(const std::wstring& team, const std::wstring& channel,
			const std::wstring& user, const std::wstring& command_text) override {

			return challenges->handle_command(team, channel, user, command_text);
			
		};
	private:
		std::shared_ptr<Matterbot> bot;
		std::shared_ptr<ChallengesCommand> challenges;
	};
}
#pragma once
