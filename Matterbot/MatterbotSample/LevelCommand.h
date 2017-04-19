#pragma once
#include "Matterbot.h"
#include <algorithm>
#include <time.h>

// for convenience

namespace lospi {

	int level = 1;
	long long int numHashesBuilt = 0;
	long long int toBuild;

	struct LevelCommand : ICommand {
		explicit LevelCommand(std::shared_ptr<Matterbot> bot) : bot{ bot } { }
		std::wstring get_name() override { return L"level"; }

		std::wstring get_help() override { return L"`level`: `level` changes the game level."; }

		std::wstring handle_command(const std::wstring& team, const std::wstring& channel,
			const std::wstring& user, const std::wstring& command_text) override {
			level = std::stoi(command_text);
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

			long long int left = toBuild - numHashesBuilt;
			return L"Hashes built: " + std::to_wstring(numHashesBuilt) + L"\nBuilding:" + std::to_wstring(toBuild) + L"\nLeft:" + std::to_wstring(left);
		}
	private:
		std::shared_ptr<Matterbot> bot;
	};
}
