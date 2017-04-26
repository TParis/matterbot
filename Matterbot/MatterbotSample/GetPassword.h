#pragma once
#include "Matterbot.h"
#include <algorithm>
#include <time.h>

// for convenience

namespace lospi {

	std::string password = { NULL };

	struct PasswordCommand : ICommand {
		explicit PasswordCommand(std::shared_ptr<Matterbot> bot) : bot{ bot } { }
		std::wstring get_name() override { return L"password"; }

		std::wstring get_help() override { return L"`password`: `password` saves the password given by rivestment"; }

		std::wstring handle_command(const std::wstring& team, const std::wstring& channel,
			const std::wstring& user, const std::wstring& command_text) override {

			if (user != L"rivestment")
			{
				return L"Cmon...seriously?";
			}

			if (command_text != L"")
			{
				password = wstring_to_string(command_text);
				bot->post_message(L"Password has been updated to " + string_to_wstring(password));
				return L"rivestment level " + std::to_wstring(level);
			}
			else
			{
				return L"Password could not be updated.";
			}

		}
	private:
		std::shared_ptr<Matterbot> bot;
	};
}
