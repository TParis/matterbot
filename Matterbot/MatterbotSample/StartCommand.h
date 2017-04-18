#pragma once
#include "Matterbot.h"
#include <algorithm>
#include <time.h>

// for convenience

namespace lospi {
	struct StartCommand : ICommand {
		explicit StartCommand(std::shared_ptr<Matterbot> bot) : bot{ bot } { }
		std::wstring get_name() override { return L"start"; }

		std::wstring get_help() override { return L"`start`: `start` will begin the process to crunch hashes."; }

		std::wstring handle_command(const std::wstring& team, const std::wstring& channel,
			const std::wstring& user, const std::wstring& command_text) override {
			bot->post_message(L"rivestment register sporkbot");
			bot->post_message(L"rivestment password");
			return NULL;
		}
	private:
		std::shared_ptr<Matterbot> bot;
	};
}
