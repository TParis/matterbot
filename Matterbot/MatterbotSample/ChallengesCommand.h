#pragma once
#include "Matterbot.h"
#include <algorithm>
#include <string>
#include <iostream>
#include <ostream>
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
#include <stdint.h>
// for convenience

namespace lospi {

	std::map<Md5Digest, std::string> hashtable;
	std::vector<std::string> challenges;
	int lospi::calcHashTotal(int level = 0);
	

	struct ChallengesCommand : public ICommand {
		explicit ChallengesCommand(std::shared_ptr<Matterbot> bot) : bot{ bot } { }

		~ChallengesCommand() {
			running = false;
		};

		std::wstring get_name() override { return L"challenges"; }

		std::wstring get_help() override { return L"`challenges`: `challenges` loads the hashes and begins crunching."; }

		std::wstring handle_command(const std::wstring& team, const std::wstring& channel,
			const std::wstring& user, const std::wstring& command_text) override {
			if (user != L"rivestment") {
				return L"Jealous much?";
			}
			running = true;
			challenges.clear();
			explode(command_text, ' ');
			
			if (hashesBuilt == false) {
				toBuild = calcHashTotal(level);
				buildHashTable(level);
				hashesBuilt = true;
			}

			std::wstring hashes{ 0 };
			hashes = checkHashes();
			hashes = trim(hashes);
			bot->post_message(L"rivestment try " + hashes);
			
			sleep(4000);
			if (lvlChanged) {
				lvlChanged = false;
				bot->post_message(L"rivestment level " + std::to_wstring(level));
			}
			return L"rivestment challenge 75";

		}

		void explode(const std::wstring input, char sep) {
			std::stringstream ss;
			std::string text = wstring_to_string(input);
			ss.str(text);
			std::string item;
			int i = 0;
			while (std::getline(ss, item, sep))
			{
				challenges.push_back(item);
			}

		}

		void buildHashTable(int level, std::string old_hash = "", int depth = 1) {
			for (int i = 0; i < 4; i++) {
				std::string alphabet = "hsoj";
				std::string tmp2 = old_hash + alphabet[i] + password;
				auto md5 = compute_md5(&tmp2[0], (unsigned long)tmp2.size());
				std::wstring mymd5_str = get_str_from_md5(md5);
				if (depth >= level - 1) {
					hashtable[md5] = old_hash + alphabet[i];
					numHashesBuilt++;
				}
				if (depth < level + 10) {
					buildHashTable(level, old_hash + alphabet[i], depth + 1);
				}
			}
		}

		std::wstring checkHashes() {

			std::wstring text;

			for (int i = 0; i < challenges.size(); i++) {
				std::string salt = hashtable[get_md5_from_str(string_to_wstring(challenges[i]))];
				text += string_to_wstring(salt);
				text += string_to_wstring(password);
				text += L" ";
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
		uint2_t to_tiny_val(std::string &digest) {
			std::map<char, char> hexmap = { { 'h', 0x0 }, { 's', 0x1 }, { 'o', 0x2 }, { 'j', 0x3 } };

			uint16_t myVal = 0x0;

			for (int i = 0; i <digest.size(); i++) {
				myVal <<= 4;
				myVal |= hexmap[digest[i]];
			}
			return myVal;
		}
	private:
		std::shared_ptr<Matterbot> bot;
	};

	//Does the same thing as the challenge command...
	struct ScrapsCommand : ICommand {
		explicit ScrapsCommand(std::shared_ptr<Matterbot> bot, std::shared_ptr<ChallengesCommand> challenges) 
		{ 
			this->bot = bot;
			this->challenges = challenges;
		};
		std::wstring get_name() override { return L"scraps"; }


		std::wstring get_help() override { return L"`scraps`: `scraps` loads the hashes and begins crunching."; }

		std::wstring handle_command(const std::wstring& team, const std::wstring& channel,
			const std::wstring& user, const std::wstring& command_text) override {

			return challenges->handle_command(team, channel, user, command_text);
			
		};
	private:
		std::shared_ptr<Matterbot> bot;
		std::shared_ptr<ChallengesCommand> challenges;
	};
}
#pragma once
