#pragma once
#include "Matterbot.h"
#include <algorithm>
#include <time.h>
#include "targetver.h"
#include "PracticalSocket.h"
#include "json.hpp"
#include <iostream>           // For cerr and cout
#include <cstdlib>            // For atoi()
#include <algorithm>

// for convenience
using json = nlohmann::json;
const int RCVBUFSIZE = 4096;    // Size of receive buffer

namespace lospi {
	struct QotdCommand : ICommand {
		explicit QotdCommand(std::shared_ptr<Matterbot> bot) : bot{ bot } { }
		std::wstring get_name() override { return L"qotd"; }

		std::wstring get_help() override { return L"`qotd`: `qotd` will give you a fancy quote."; }

		std::wstring handle_command(const std::wstring& team, const std::wstring& channel,
			const std::wstring& user, const std::wstring& command_text) override {
			string omdb = getData();
			string JSON = isolateJSON(&omdb);
			wstring mystr;

			if (user == L"bean")
			{
				return L"WAKE UP BEAN!";
			}

			try
			{
				auto quoteObj = json::parse(JSON);
				std::string quote = quoteObj["content"];
				std::string author = quoteObj["title"];
				std::string text = "\"" + quote.substr(0, quote.size() - 1) + "\" - " + author;
				StringToWString(mystr, conv_utf8(remove_html(filter_chars(text))));
				return mystr;
			}
			catch (exception& e)
			{
				std::string error = e.what();
				wclog << (std::wstring&)error << endl;
				return L"Error while processing Qotd";
			}
		}


		std::string ReplaceAll(std::string str, const std::string& from, const std::string& to)
		{
			size_t start_pos = 0;
			while ((start_pos = str.find(from, start_pos)) != std::string::npos)
			{
				str.replace(start_pos, from.length(), to);
				start_pos += to.length(); // Handles case where 'to' is a substring of 'from'
			}
			return str;
		}

		int StringToWString(std::wstring &ws, const std::string &s)
		{
			std::wstring wsTmp(s.begin(), s.end());

			ws = wsTmp;

			return 0;
		}

		string buildRequest()
		{
			string request;

			request = "GET /wp-json/posts?filter[orderby]=rand&filter[posts_per_page]=1 HTTP/1.1\r\n";
			request += "Connection: keep-alive\r\n";
			request += "Host: www.quotesondesign.com\r\n";
			request += "User-Agent: My C++ Script Amazon Data Importer For School\r\n";
			request += "Accept-Language: en-US,en;q=0.8\r\n";
			request += "Upgrade-Insecure-Requests: 1\r\n";
			request += "\r\n";

			return request;
		}


		string getData()
		{

			string servAddress = "quotesondesign.com"; // First arg: server address
			string recvMessage;
			unsigned short echoServPort = 80;

			try
			{
				// Establish connection with the echo server
				TCPSocket sock(servAddress, echoServPort);

				// Get Request
				string request = buildRequest();
				char *http_req = const_cast<char*>(request.c_str());

				// Send the string to the echo server
				sock.send(http_req, strlen(http_req));

				char responseBuff[RCVBUFSIZE + 1] = { 0 };
				int recvMsgSize;

				while ((recvMsgSize = sock.recv(responseBuff, RCVBUFSIZE)) > 0) { // Zero means

					recvMessage += responseBuff;

					//If there is no more content in the stream, leave while loop
					if (responseBuff[0] == '\0' || RCVBUFSIZE > recvMsgSize)
					{
						break;
					}
					//If there is still content left, then clear the buffer for new content
					else
					{
						memset(&responseBuff, '\0', sizeof(responseBuff));
					}
				}

			}
			catch (SocketException &e)
			{
				cerr << e.what() << endl;
				exit(1);
			}

			return recvMessage;
		}

		string isolateJSON(string *response)
		{
			return (*response).substr((*response).find("{"), (*response).size() - (*response).find("{") - 1);
		}

		string removeQuotes(string s) {
			s.erase(
				remove(s.begin(), s.end(), '\"'),
				s.end()
			);
			return s;
		}
		std::string filter_chars(string data)
		{
			int i = 0, j = 0;
			while (i<data.length())
			{
				if ((data[i] >= 'A' && data[i] <= 'Z') || (data[i] >= 'a' || data[i] <= 'z') || data[i] == ' ' || data[i] == '.' || (data[i] >= '0' && data[i] <= '9'))
				{
					data[i] = data[i];
					i++;
				}
				else
				{
					data.erase(i, 1);
				}
			}
			return data;
		}

		std::string remove_html(string buf) {
			// assuming 'buf' contains your to-be-stripped html

			char *newbuf = new char[buf.size()];
			int len = buf.size();
			int j = 0;

			// scan string
			for (int i = 0; i<len; i++)
			{
				// found an open '<', scan for its close
				if (buf[i] == '<')
				{
					// charge ahead in the string until it runs out or we find what we're looking for
					for (; i<len && buf[i] != '>'; i++);
				}
				else
				{
					newbuf[j++] = buf[i];
				}
			}

			newbuf[j] = 0;
			buf = newbuf;
			delete[] newbuf;
			return buf;
		}

		std::string conv_utf8(string buf) {
			// assuming 'buf' contains your to-be-stripped html

			char *newbuf = new char[buf.size()];
			int len = buf.size();
			int j = 0;

			// scan string
			for (int i = 0; i<len; i++)
			{
				// found an open '<', scan for its close
				if (buf[i] == '&' && buf[i + 1] && buf[i+1] == '#')
				{
					i++; //Passes the '#'
					// charge ahead in the string until it runs out or we find what we're looking for
					for (; i < len && buf[i] != ';'; i++) {
					};
					newbuf[j++] = '\'';
				}
				else
				{
					newbuf[j++] = buf[i];
				}
			}

			newbuf[j] = 0;
			buf = newbuf;
			return buf;
		}
	private:
		std::shared_ptr<Matterbot> bot;
	};
}
