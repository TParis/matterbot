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

namespace lospi {
	struct WeatherCommand : ICommand {
		explicit WeatherCommand(std::shared_ptr<Matterbot> bot) : bot{ bot } { }
		std::wstring get_name() override { return L"weather"; }

		std::wstring get_help() override { return L"`weather`: `weather` will give you a the weather for San Antonio."; }

		std::wstring handle_command(const std::wstring& team, const std::wstring& channel,
			const std::wstring& user, const std::wstring& command_text) override {
			string omdb = getData();
			string JSON = isolateJSON(&omdb);
			wstring mystr;


			try {
				auto quoteObj = json::parse(JSON);
				string sunrise = quoteObj["query"]["results"]["channel"]["astronomy"]["sunrise"];
				if (!isdigit(sunrise[3])) {
					sunrise.resize(sunrise.size() + 1, '\0');
					sunrise[6] = sunrise[5];
					sunrise[5] = sunrise[4];
					sunrise[4] = ' ';
					sunrise[3] = sunrise[2];
					sunrise[2] = '0';
				}
				string sunset = quoteObj["query"]["results"]["channel"]["astronomy"]["sunset"];
				if (!isdigit(sunset[3])) {
					sunset.resize(sunset.size() + 1, '\0');
					sunset[6] = sunset[5];
					sunset[5] = sunset[4];
					sunset[4] = ' ';
					sunset[3] = sunset[2];
					sunset[2] = '0';
				}
				string high = quoteObj["query"]["results"]["channel"]["item"]["forecast"][0]["high"];
				string low = quoteObj["query"]["results"]["channel"]["item"]["forecast"][0]["low"];
				string text = quoteObj["query"]["results"]["channel"]["item"]["forecast"][0]["text"];
				std::string weather = "";
				weather = weather + "Sunrise: " + sunrise + "\n";
				weather = weather + "Sunset: " + sunset + "\n";
				weather = weather + "TODAY:\n";
				weather = weather + "Temp High: " + high + "F\n";
				weather = weather + "Temp Low: " + low + "F\n";
				weather = weather + "Weather: " + text + "\n";
				high = quoteObj["query"]["results"]["channel"]["item"]["forecast"][1]["high"];
				low = quoteObj["query"]["results"]["channel"]["item"]["forecast"][1]["low"];
				text = quoteObj["query"]["results"]["channel"]["item"]["forecast"][1]["text"];
				weather = weather + "TOMORROW:\n";
				weather = weather + "Temp High: " + high + "F\n";
				weather = weather + "Temp Low: " + low + "F\n";
				weather = weather + "Weather: " + text + "\n";
				StringToWString(mystr, weather);
				return mystr;
			}
			catch (exception& e) {
				std::string error = e.what();
				wclog << (std::wstring&)error << endl;
				return L"";
			}
		}

		std::string ReplaceAll(std::string str, const std::string& from, const std::string& to) {
			size_t start_pos = 0;
			while ((start_pos = str.find(from, start_pos)) != std::string::npos) {
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

		string buildRequest() {
			string request;

			request = "GET /v1/public/yql?q=select%20*%20from%20weather.forecast%20where%20woeid%20in%20(select%20woeid%20from%20geo.places(1)%20where%20text%3D%22San%20Antonio%2C%20TX%22)&format=json&env=store%3A%2F%2Fdatatables.org%2Falltableswithkeys HTTP/1.1\r\n";
			request += "Connection: keep-alive\r\n";
			request += "Host: query.yahooapis.com\r\n";
			request += "User-Agent: My C++ Script Amazon Data Importer For School\r\n";
			request += "Accept-Language: en-US,en;q=0.8\r\n";
			request += "Upgrade-Insecure-Requests: 1\r\n";
			request += "\r\n";

			return request;
		}


		string getData() {

			string servAddress = "query.yahooapis.com"; // First arg: server address
			string recvMessage;
			unsigned short echoServPort = 80;
			try {
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
			catch (SocketException &e) {
				cerr << e.what() << endl;
				exit(1);
			}

			return recvMessage;
		}

		string isolateJSON(string *response) {

			string resp = *response;
			return resp.substr(resp.find("{"));

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
