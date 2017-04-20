#pragma once
#include "Matterbot.h"
#include <algorithm>
#include <string>
#include <iostream>
#include <ostream>
#include <time.h>
#include <vector>
#include <thread>
#include <chrono>
#include <map>
#include "Md5.h"
#include "Md5Utilities.h"
#include "GetPassword.h"
#include <functional> 
#include <cctype>
#include <locale>
// for convenience

namespace lospi {

	std::map<std::string, std::string> hashtable;
	std::vector<std::wstring> challenges;

	struct ChallengesCommand : ICommand {
		explicit ChallengesCommand(std::shared_ptr<Matterbot> bot) : bot{ bot } { }
		std::wstring get_name() override { return L"challenges"; }

		std::wstring get_help() override { return L"`challenges`: `challenges` loads the hashes and begins crunching."; }

		std::wstring handle_command(const std::wstring& team, const std::wstring& channel,
			const std::wstring& user, const std::wstring& command_text) override {
			
			
			challenges = explode(command_text, L' ');
			
			if (hashesBuilt == false) {
				toBuild = pow(5, level + 10);
				buildHashTable(level);
				hashesBuilt = true;
			}

			std::wstring hashes{ 0 };
			hashes = checkHashes();
			hashes = trim(hashes);
			bot->post_message(L"rivestment try " + hashes);
			
			sleep(3000);
			if (lvlChanged) {
				bot->post_message(L"rivestment level " + std::to_wstring(level));
			}
			return L"rivestment challenge";

		}

		std::vector<std::wstring> explode(const std::wstring input, wchar_t sep) {
			std::wstring tmp;
			std::vector<std::wstring> output;
			tmp.resize(32);
			int i = 0;
			int j = 0;
			while(input[i]) {
				if (input[i] == sep) {
					output.push_back(tmp);
					tmp.clear();
					tmp.resize(32);
					j = 0;
				}
				else {
					tmp[j++] = input[i];
				}
				i++;
			}
			output.push_back(tmp);

			return output;
		}

		void buildHashTable(int level, std::wstring old_hash = L"", int depth = 1) {
			try {
				for (int i = 0; i < 4; i++) {
					std::wstring alphabet = L"hsoj";
					std::wstring new_hash = old_hash + alphabet[i];
					std::wstring tmp = new_hash + password;
					std::string tmp2 = wstring_to_string(tmp);
					char tmp3[33] = { 0 };
					strcpy(tmp3, tmp2.c_str());
					auto md5 = compute_md5(tmp3, tmp2.size());
					std::wstring mymd5_str = get_str_from_md5(md5);
					if (depth >= level) {
						hashtable[wstring_to_string(mymd5_str)] = wstring_to_string(tmp);
						numHashesBuilt++;
					}
					if (depth <= level + 10 && numHashesBuilt < toBuild) {
						buildHashTable(level, new_hash, depth + 1);
					}
				}
			}
			catch (std::exception e) {
				bot->post_message(L"Could not execute after " + std::to_wstring(numHashesBuilt) + L" with error" + string_to_wstring(std::string(e.what())));
			}
		}

		std::wstring checkHashes() {

			std::wstring text;

			for (int i = 0; i < challenges.size(); i++) {
				std::wstring idx = challenges[i];
				std::string hash = wstring_to_string(idx);
				if (hash != "") {
					std::string salt = hashtable[hash];
					text += string_to_wstring(salt);
					text += L" ";
				}
			}

			return text;
		}

		/*void checkHashes(std::vector<std::wstring> hashes) {
			

			for (int i = 0; i < hashes.size(); i++) {
				
				std::wstring retn = crackHash(hashes[i]);

				if (retn != L"")
				{
					bot->post_message(L"rivestment try " + retn);
					sleep(1000);
				}
				else
				{
					bot->post_message(L"Could not crack " + hashes[i]);
					sleep(1000);
				}

			}
		}

		std::wstring crackHash(std::wstring hash, std::wstring old_brute = L"", int depth = 0) {

			for (int i = 0; i < 4; i++) {
				std::wstring alphabet = L"hsoj";
				std::wstring new_brute = old_brute + alphabet[i];
				std::wstring tmp = new_brute + password;
				std::string tmp2 = wstring_to_string(tmp);
				char * tmp3 = (char *)calloc(1, tmp2.size());
				strcpy(tmp3, tmp2.c_str());
				auto md5 = compute_md5(tmp3, tmp2.size());
				std::wstring mymd5_str = get_str_from_md5(md5);
				if (mymd5_str == hash)
				{
					return tmp;
				}
				else if (depth < 10)
				{
					std::wstring retn = crackHash(hash, new_brute, depth + 1);
					if (retn != L"") {
						return retn;
					}
				}
			}
			return L"";
		}*/
		void sleep(int time) {
			std::this_thread::sleep_for(std::chrono::milliseconds(time));
		}
		// trim from start
		static inline std::wstring &ltrim(std::wstring &s) {
			s.erase(s.begin(), std::find_if(s.begin(), s.end(),
				std::not1(std::ptr_fun<int, int>(std::isspace))));
			return s;
		}

		// trim from end
		static inline std::wstring &rtrim(std::wstring &s) {
			s.erase(std::find_if(s.rbegin(), s.rend(),
				std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
			return s;
		}

		// trim from both ends
		static inline std::wstring &trim(std::wstring &s) {
			return ltrim(rtrim(s));
		}
	private:
		std::shared_ptr<Matterbot> bot;
	};
}
#pragma once
