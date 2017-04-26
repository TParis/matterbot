#pragma once
#include "Matterbot.h"
#include <algorithm>
#include <time.h>
#include "Md5.h"
#include "Md5Utilities.h"

// for convenience

namespace lospi {

	int timer = 2100;

	struct TimerCommand : ICommand {
		explicit TimerCommand(std::shared_ptr<Matterbot> bot) : bot{ bot } { }
		std::wstring get_name() override { return L"timer"; }

		std::wstring get_help() override { return L"`timer`: `timer` changes the interval between challenges (Default: 2100)."; }

		std::wstring handle_command(const std::wstring& team, const std::wstring& channel,
			const std::wstring& user, const std::wstring& command_text) override {

			if (user != L"tparis00ap")
			{
				return L"I'm freaky fast, like Jimmy Johns.";
			}

			if (std::stoi(command_text) > 0)
			{
				timer = std::stoi(command_text);
				return L"Timer has been changed to " + std::to_wstring(timer);
			}
			else
			{
				return L"Error updating timer";
			}

		}
	private:
		std::shared_ptr<Matterbot> bot;
	};
}
