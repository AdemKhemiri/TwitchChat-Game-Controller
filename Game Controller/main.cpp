#include <WinSock2.h>
#include <WS2tcpip.h>
#include <Windows.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <regex>

#include <KeyboardInput.h> 

#include "TwitchSocket.h"


#pragma comment(lib, "Ws2_32.lib")
#pragma comment(lib, "Winmm.lib")

void rocketLeagueControl(std::string&);


tobInput::KeyboardInput keyboard;
tobInput::MouseInput mouse;

int main()
{
	WSADATA wsaData;
	SOCKET Socket = INVALID_SOCKET;
	struct addrinfo hints {};  /// COULD REMOVE THE STRUCT
	addrinfo* result;
	std::ifstream file("oathToken.txt");
	std::string OAuthToken;
	if (file.is_open())
	{
		std::getline(file, OAuthToken, '\n');
		file.close();
	}
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		std::cerr << "Startup Failed." << std::endl;
		return 1;
	}

	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	if (getaddrinfo("irc.chat.twitch.tv", "6667", &hints, &result) != 0)
	{
		std::cerr << "getaddrinfo failed: <<RETURN CODE CAN BE PASSED>>" << std::endl;
	}
	Socket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	if (Socket == INVALID_SOCKET)
	{
		std::cerr << "ERROR at socket" << WSAGetLastError << std::endl;
		freeaddrinfo(result);
		WSACleanup();
		return 1;
	}
	if (connect(Socket, result->ai_addr, result->ai_addrlen) == SOCKET_ERROR)
	{
		closesocket(Socket);
		Socket = INVALID_SOCKET;
	}
	freeaddrinfo(result);
	if (Socket == INVALID_SOCKET)
	{
		std::cerr << "Cannot connect to server" << std::endl;
		WSACleanup();
		return 1;
	}

	gh::TwitchSocket twitch(Socket);
	twitch.send("PASS " + OAuthToken + "\r\n");
	twitch.send("NICK adam79_kh\r\n"); //// ADD THE CHANNELNAME IN LOWER CASE

	//(void)twitch.receive();
	//std::cout << twitch.receive() << std::endl;
	twitch.send("JOIN #adam79_kh\r\n");

	// Parse message
	std::regex re("!(.+)@.+ PRIVMSG #([^\\s]+) :(.*)");
	std::smatch match;
	while (!GetAsyncKeyState(VkKeyScanA('b'))&&!GetAsyncKeyState(VkKeyScan(VK_LCONTROL)))
	{
		Sleep(500);
		std::string reply = twitch.receive();
		std::regex_search(reply, match, re);
		std::string userName = match[1];
		std::string msg = match[3];
		std::cout << "message: " << msg << std::endl;
		rocketLeagueControl(msg);	////////// GAME CONTROLLER ///////////


	}
	return 0;
}
void rocketLeagueControl(std::string& msg)
{
	if (msg == "up")
	{
		keyboard.Press('z');
		
	}
	if (msg == "down")
	{
		keyboard.Press('s');
	}
	if (msg == "left")
	{
		keyboard.Press('z');
		keyboard.Press('q');
	}
	if (msg == "right")
	{
		keyboard.Press('z');
		keyboard.Press('d');
	}
	if (msg == "ball")
	{
		keyboard.VKPress(VK_SPACE);
	}
	if (msg == "jump")
	{
		mouse.RMouseClick();
	}
	if (msg == "boost")
	{
		mouse.LMouseClick();
	}
}