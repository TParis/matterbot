#pragma once
#include "Matterbot.h"
#include <algorithm>
#include <time.h>

// for convenience

namespace lospi {
	struct RestartCommand : ICommand {
		explicit RestartCommand(std::shared_ptr<Matterbot> bot) : bot{ bot } { }
		std::wstring get_name() override { return L"restart"; }

		std::wstring get_help() override { return L"`restart`: `restart` will restart the process to crunch hashes."; }

		std::wstring handle_command(const std::wstring& team, const std::wstring& channel,
			const std::wstring& user, const std::wstring& command_text) override {


			if (user != L"tparis00ap")
			{
				return L"Yeah, nope...";
			}
			
			bot->post_message(L"bananabot password");
			
			if (lvlChanged)
			{
				lvlChanged = false;
				bot->post_message(L"bananabot level " + std::to_wstring(level));
			}

			sleep(timer);

			return L"bananabot scraps";
		}
	private:
		std::shared_ptr<Matterbot> bot;
	};
}
