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

	std::map<Md5Digest, uint64_t> hashtable;
	std::vector<std::string> challenges;
	std::string alphabet = "hsoj";
	

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
			
			sleep(timer);

			if (lvlChanged) {
				lvlChanged = false;
				bot->post_message(L"rivestment level " + std::to_wstring(level));
			}
			return L"rivestment challenge " + std::to_wstring(number);

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

		void buildHashTable(int level) {

			for (int i = level - 1; i < level + 10; i++) {
				auto combos = Combinator("hsoj", i);
				while (combos.has_next()) {
					auto c = combos.next();
					std::string salt(c.begin(), c.end());
					std::copy(password.begin(), password.end(), back_inserter(c));
					hashtable[compute_md5(c.data(), c.size())] = to_tiny_val(salt.substr(0, i));
					numHashesBuilt++;
				}
			}
		}

		int calcHashTotal(int level = 1) {
			int total = 0;
			for (int i = level - 1; i < level + 10; i++) {
				total += (int)pow(4, i);
			}
			return total;
		}

		std::wstring checkHashes() {

			std::wstring text;

			for (int i = 0; i < challenges.size(); i++) {
				std::string salt = from_tiny_val(hashtable[get_md5_from_str(string_to_wstring(challenges[i]))]);
				text += string_to_wstring(salt);
				text += string_to_wstring(password);
				text += L" ";
			}

			return text;
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
		uint64_t to_tiny_val(std::string &digest) {
			std::map<char, char> hexmap = { { 'h', 0x1 }, { 's', 0x2 }, { 'o', 0x3 }, { 'j', 0x4 } };

			uint64_t myVal = 0x0;

			for (int i = 0; i <digest.size(); i++) {
				myVal <<= 3;
				myVal |= hexmap[digest[i]];
			}
			return myVal;
		}

		std::string from_tiny_val(uint64_t myInt) {

			std::map<char, char> hexmap = { { 0x0, 0x0}, {0x1, 'h' }, { 0x2, 's' }, { 0x3, 'o' }, { 0x4, 'j' } };

			std::string myVal;

			for (int i = 0; i < sizeof(uint64_t) * 4; i++) {
				if (hexmap[myInt & 0x7] != 0x0) {
					myVal += hexmap[myInt & 0x7];
				}
				myInt >>= 3;
			}
			reverse(myVal.begin(), myVal.end());
			return myVal;
		}
	private:
		std::shared_ptr<Matterbot> bot;
	};
}
