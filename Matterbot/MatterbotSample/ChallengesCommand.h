#pragma once
#include "Matterbot.h"
#include <algorithm>
#include <time.h>
#include <vector>
#include "Md5.h"
#include "Md5Utilities.h"
#include "GetPassword.h"
// for convenience

namespace lospi {

	std::vector<std::string> hashtable[2];

	struct ChallengesCommand : ICommand {
		explicit ChallengesCommand(std::shared_ptr<Matterbot> bot) : bot{ bot } { }
		std::wstring get_name() override { return L"challenges"; }

		std::wstring get_help() override { return L"`challenges`: `challenges` loads the hashes and begins crunching."; }

		std::wstring handle_command(const std::wstring& team, const std::wstring& channel,
			const std::wstring& user, const std::wstring& command_text) override {


			std::vector<std::wstring> challenges;
			
			explode(command_text, ' ', challenges);
			
			checkHashes(challenges);
		}

		void explode(const std::wstring input, char sep, std::vector<std::wstring> output) {
			std::wstring tmp;
			tmp.resize(33);
			tmp[32] = '\0';
			int i = 0;
			int j = 0;
			while(input[i] != EOF) {
				if (input[i] == sep) {
					output.push_back(tmp);
					tmp = L"";
					j = 0;
				}
				else {
					tmp[j++] = input[i];
				}
				i++;
			}
		}

		void checkHashes(std::vector<std::wstring> hashes) {
			

			for (int i = 0; i < hashes.size(); i++) {
				crackHash(hashes[i]);
			}
		}

		void crackHash(std::wstring hash, std::wstring brute = L"", int depth = 0) {

			for (int i = 0; i < 4; i++) {
				std::wstring alphabet = L"hsoj";
				brute = brute + alphabet[i];
				std::wstring tmp = hash + password;
				auto md5 = get_md5_from_str(tmp);
				unsigned char * mymd5 = md5.data();
				/*std::string mymd5_str (mymd5, 32);
				if (mymd5_str == wstring_to_string(hash)) {
					return hash
				}*/
			}
		}
	private:
		std::shared_ptr<Matterbot> bot;
	};
}
#pragma once
