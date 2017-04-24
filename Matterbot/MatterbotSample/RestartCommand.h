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


			if (user != L"tparis00ap") {
				return L"Yeah, nope...";
			}
			
			bot->post_message(L"rivestment password");
			
			if (lvlChanged) {
				lvlChanged = false;
				bot->post_message(L"rivestment level " + std::to_wstring(level));
			}
			sleep(3000);
			bot->post_message(L"rivestment scraps");
			return L"";
		}
	private:
		std::shared_ptr<Matterbot> bot;
	};
	struct HardRestartCommand : ICommand {
		explicit HardRestartCommand(std::shared_ptr<Matterbot> bot) : bot{ bot } { }
		std::wstring get_name() override { return L"hrestart"; }

		std::wstring get_help() override { return L"`restart`: `restart` will restart the process to crunch hashes."; }

		std::wstring handle_command(const std::wstring& team, const std::wstring& channel,
			const std::wstring& user, const std::wstring& command_text) override {


			if (user != L"tparis00ap") {
				return L"Yeah, nope...";
			}
			
			bot->post_message(L"rivestment quit");
			bot->post_message(L"rivestment register sporkbot");
			bot->post_message(L"rivestment password");

			if (lvlChanged) {
				lvlChanged = false;
				bot->post_message(L"rivestment level " + std::to_wstring(level));
			}

			sleep(3000);
			return L"rivestment challenge " + std::to_wstring(number);
		}
	private:
		std::shared_ptr<Matterbot> bot;
	};
}
