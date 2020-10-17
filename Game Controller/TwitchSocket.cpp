#include <vector>
#include <string>
#include <Winsock2.h>
#include <Windows.h>
#include "TwitchSocket.h"

namespace gh {
	int TwitchSocket::send(const std::string& information)
	{
		return ::send(Socket, information.c_str(), information.length(), NULL);
	}
	std::string TwitchSocket::receive()
	{
		int byteRecieved = recv(Socket, buffer.data(), bufferLength, NULL);
		//std::cout << buffer.data() << std::endl;
		if (byteRecieved <= 0)
			return "Vector empty";
		else
			return std::string(buffer.begin(), buffer.begin() + byteRecieved);

	}
	TwitchSocket::TwitchSocket(SOCKET socket)
	{
		this->Socket = socket;
	}
}