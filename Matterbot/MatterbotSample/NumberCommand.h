#pragma once
#include "Matterbot.h"
#include <algorithm>
#include <time.h>

// for convenience

namespace lospi {

	int number = 175;

	struct NumberCommand : ICommand {
		explicit NumberCommand(std::shared_ptr<Matterbot> bot) : bot{ bot } { }
		std::wstring get_name() override { return L"number"; }

		std::wstring get_help() override { return L"`number`: `number` changes the number of challenges requested by rivestment"; }

		std::wstring handle_command(const std::wstring& team, const std::wstring& channel,
			const std::wstring& user, const std::wstring& command_text) override {
			if (user != L"tparis00ap") {
				return L"I demand the sum... OF 1 MILLION DOLLARS. ";
			}
			number = std::stoi(command_text);
			return L"Challenges requested changed to " + std::to_wstring(number);
		}
	private:
		std::shared_ptr<Matterbot> bot;
	};
}
