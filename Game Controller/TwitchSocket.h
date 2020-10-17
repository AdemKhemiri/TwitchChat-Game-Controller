#include <vector>
#include <string>
#include <Winsock2.h>
#include <Windows.h>

#include <iostream>

#pragma once

namespace gh {
	constexpr int bufferLength = 512;

	class TwitchSocket {
	private:
		std::vector<char> buffer = std::vector<char>(bufferLength);
		SOCKET Socket;
	public:
		int send(const std::string& information);
		std::string receive();
		TwitchSocket(SOCKET socket);

		TwitchSocket(const TwitchSocket&) = delete;
		TwitchSocket& operator=(const TwitchSocket&) = delete;
	};
}

