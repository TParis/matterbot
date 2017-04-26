#pragma once
#include "Matterbot.h"
#include <algorithm>
#include <time.h>
#define ERROR_SETTING_VARS -1

// for convenience

namespace lospi {

	bool running = false;

	struct StartCommand : ICommand {
		explicit StartCommand(std::shared_ptr<Matterbot> bot) : bot{ bot } { }
		std::wstring get_name() override { return L"start"; }

		std::wstring get_help() override {

			std::wstring response { 0 };

			response = L"`start`: `start` will begin the process to crunch hashes.\n";
			response += L"\t`-t`: `-t [number]` sets the timer interval\n";
			response += L"\t`-n`: `-n [number]` sets challenges requested\n";
			response += L"\t`-l`: `-l [number]` sets level requested\n";
			return response;
		}

		std::wstring handle_command(const std::wstring& team, const std::wstring& channel,
			const std::wstring& user, const std::wstring& command_text) override {

			if (running)
			{
				return L"I'm already running, silly";
			}

			if (!processFlags(command_text)) {
				return L"Error processing flags";
			}

			bot->post_message(L"rivestment register sporkbot");

			bot->post_message(L"rivestment password");

			if (lvlChanged)
			{
				bot->post_message(L"rivestment level " + std::to_wstring(level));
				lvlChanged = false;
			}

			sleep(timer);

			return L"rivestment challenge " + std::to_wstring(number);
		}

		int processFlags(std::wstring command_text) {

			std::vector<std::string> command_array;
			command_array = explode(wstring_to_string(command_text), ' ');

			for (int i = 0; i < command_array.size(); i++) {
				if (command_array[i] == "-t")
				{
					if (std::stoi(command_array[i + 1]) > 0)
					{
						timer = std::stoi(command_array[i + 1]);
					}
					else
					{
						return ERROR_SETTING_VARS;
					}
					i++;
				}
				else if (command_array[i] == "-n")
				{
					if (std::stoi(command_array[i + 1]) > 0 && std::stoi(command_array[i + 1]) < 350)
					{
						number = std::stoi(command_array[i + 1]);
					}
					else
					{
						return ERROR_SETTING_VARS;
					}
					i++;
				}
				else if (command_array[i] == "-l")
				{
					if (std::stoi(command_array[i + 1]) > 0 && std::stoi(command_array[i + 1]) < 6)
					{
						level = std::stoi(command_array[i + 1]);
						lvlChanged = true;
					}
					else
					{
						return ERROR_SETTING_VARS;
					}
					i++;
				}
			}

		}
	private:
		std::shared_ptr<Matterbot> bot;
	};
}