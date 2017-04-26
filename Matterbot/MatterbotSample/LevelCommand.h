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
	bool hashesBuilt = false;

	struct LevelCommand : ICommand {
		explicit LevelCommand(std::shared_ptr<Matterbot> bot) : bot{ bot } { }
		std::wstring get_name() override { return L"level"; }

		std::wstring get_help() override { return L"`level`: `level` changes the game level."; }

		std::wstring handle_command(const std::wstring& team, const std::wstring& channel,
			const std::wstring& user, const std::wstring& command_text) override {

			if (user != L"tparis00ap")
			{
				return L"Level has been changed to your mom riding a tyrannosaurus shooting a bazooka at flamingos.";
			}

			if (std::stoi(command_text) > 0 && std::stoi(command_text) < 6)
			{
				level = std::stoi(command_text);
				lvlChanged = true;
				hashesBuilt = false;
				return L"Level has been changed to " + std::to_wstring(level);
			}
			else
			{
				return L"Cannot change the level to number specified";
			}

		}
	private:
		std::shared_ptr<Matterbot> bot;
	};
}
