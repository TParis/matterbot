#pragma once
#include "Matterbot.h"
#include <algorithm>
#include <time.h>
#include "Md5.h"
#include "Md5Utilities.h"

// for convenience

namespace lospi {

	int level = 1;
	bool lvlChanged = false;
	long unsigned int numHashesBuilt = 0;
	long unsigned int toBuild;
	long unsigned int left;
	bool hashesBuilt = false;

	struct LevelCommand : ICommand {
		explicit LevelCommand(std::shared_ptr<Matterbot> bot) : bot{ bot } { }
		std::wstring get_name() override { return L"level"; }

		std::wstring get_help() override { return L"`level`: `level` changes the game level."; }

		std::wstring handle_command(const std::wstring& team, const std::wstring& channel,
			const std::wstring& user, const std::wstring& command_text) override {
			level = std::stoi(command_text);
			lvlChanged = true;
			hashesBuilt = 0;
			return L"Level has been changed to " + std::to_wstring(level);
		}
	private:
		std::shared_ptr<Matterbot> bot;
	};
	struct StatusCommand : ICommand {
		explicit StatusCommand(std::shared_ptr<Matterbot> bot) : bot{ bot } { }
		std::wstring get_name() override { return L"status"; }

		std::wstring get_help() override { return L"`level`: `level` changes the game level."; }

		std::wstring handle_command(const std::wstring& team, const std::wstring& channel,
			const std::wstring& user, const std::wstring& command_text) override {

			left = toBuild - numHashesBuilt;
			std::string response = "```Hashes built: ";
			response += std::to_string(numHashesBuilt);
			response += "\nBuilding:     ";
			response += std::to_string(toBuild);
			response += "\nLeft:         ";
			response += std::to_string(left);
			response += "```";
			return string_to_wstring(response);
		}
	private:
		std::shared_ptr<Matterbot> bot;
	};
}
