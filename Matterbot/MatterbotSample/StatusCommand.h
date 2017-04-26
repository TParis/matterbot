#pragma once
#include "Matterbot.h"
#include <algorithm>
#include <time.h>
#include "Md5.h"
#include "Md5Utilities.h"

// for convenience

namespace lospi {

	long unsigned int numHashesBuilt = 0;
	long unsigned int toBuild;
	long unsigned int left;

	struct StatusCommand : ICommand {
		explicit StatusCommand(std::shared_ptr<Matterbot> bot) : bot{ bot } { }
		std::wstring get_name() override { return L"status"; }

		std::wstring get_help() override { return L"`status`: `status` gives an update about the has table building."; }

		std::wstring handle_command(const std::wstring& team, const std::wstring& channel,
			const std::wstring& user, const std::wstring& command_text) override {
			
			left = toBuild - numHashesBuilt;
			double prct = ((double)numHashesBuilt / toBuild) * 100.00;

			std::string response = "```Hashes built: ";
			response += std::to_string(numHashesBuilt);
			response += "\nBuilding:     ";
			response += std::to_string(toBuild);
			response += "\nLeft:         ";
			response += std::to_string(left);
			response += "\nPercent:      ";
			response += std::to_string(prct);
			response += "%```";

			return string_to_wstring(response);
		}
	private:
		std::shared_ptr<Matterbot> bot;
	};
}
