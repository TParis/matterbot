#pragma once
#include "Matterbot.h"
#include <algorithm>
#include <time.h>

// for convenience

namespace lospi {
	struct StartCommand : ICommand {
		explicit StartCommand(std::shared_ptr<Matterbot> bot) : bot{ bot } { }
		std::wstring get_name() override { return L"restart"; }

		std::wstring get_help() override { return L"`restart`: `restart` will restart the process to crunch hashes."; }

		std::wstring handle_command(const std::wstring& team, const std::wstring& channel,
			const std::wstring& user, const std::wstring& command_text) override {
			bot->post_message(L"rivestment quit");
			bot->post_message(L"rivestment register sporkbot");
			bot->post_message(L"rivestment password");
			return L"";
		}
	private:
		std::shared_ptr<Matterbot> bot;
	};
}
