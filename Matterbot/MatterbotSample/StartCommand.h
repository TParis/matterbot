#pragma once
#include "Matterbot.h"
#include <algorithm>
#include <time.h>

// for convenience

namespace lospi {


	bool running = false;


	struct StartCommand : ICommand {
		explicit StartCommand(std::shared_ptr<Matterbot> bot) : bot{ bot } { }
		std::wstring get_name() override { return L"start"; }

		std::wstring get_help() override { return L"`start`: `start` will begin the process to crunch hashes."; }

		std::wstring handle_command(const std::wstring& team, const std::wstring& channel,
			const std::wstring& user, const std::wstring& command_text) override {

			if (running == true) {
				return L"I'm already running, silly";
			}

			bot->post_message(L"rivestment register sporkbot");
			bot->post_message(L"rivestment password");

			if (lvlChanged) {
				lvlChanged = false;
				bot->post_message(L"rivestment level " + std::to_wstring(level));
			}

			return L"rivestment challenge";
		}
	private:
		std::shared_ptr<Matterbot> bot;
	};
}