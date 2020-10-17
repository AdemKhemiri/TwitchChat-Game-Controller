#include <WinSock2.h>
#include <WS2tcpip.h>
#include <Windows.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <regex>

#include "TwitchSocket.h"

#pragma comment(lib, "Ws2_32.lib")
#pragma comment(lib, "Winmm.lib")

void dxKeyPressed(const char&);
void keyboardPressed(const char& key);

void gameControl(std::string&);
void lockBall();
void jumpPressed();
void boostPressed();
void dxBoost();
void dxJump();

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

	(void)twitch.receive();
	//std::cout << twitch.receive() << std::endl;
	twitch.send("JOIN #adam79_kh\r\n");

	// Parse message
	std::regex re("!(.+)@.+ PRIVMSG #([^\\s]+) :(.*)");
	std::smatch match;
	while (true)
	{
		Sleep(500);
		std::string reply = twitch.receive();
		std::regex_search(reply, match, re);
		std::string userName = match[1];
		std::string msg = match[3];
		std::cout << "message: " << msg << std::endl;
		gameControl(msg);	////////// GAME CONTROLLER ///////////


	}
	return 0;
}
void gameControl(std::string& msg)
{
	if (msg == "up")
	{
		//keyboardPressed('z');
		dxKeyPressed('z');
		//std::cout << "Z SHOULD BE PRESSEDDDDDDDDDDDDDDDD" << std::endl;
	}
	if (msg == "down")
	{
		//keyboardPressed('s');
		dxKeyPressed('s');
	}
	if (msg == "left")
	{
		dxKeyPressed('z');
		dxKeyPressed('q');
	}
	if (msg == "right")
	{
		dxKeyPressed('z');
		dxKeyPressed('d');
	}
	if (msg == "ball")
	{
		lockBall();
	}
	if (msg == "jump")
	{
		//dxKeyPressed('k');
		dxJump();
	}
	if (msg == "boost")
	{
		//dxKeyPressed('j');
		dxBoost();
	}
}
void keyboardPressed(const char& key)
{
	INPUT input = { 0 };
	input.type = INPUT_KEYBOARD;
	input.ki.wVk = VkKeyScanA(key);
	SendInput(1, &input, sizeof(input));
	ZeroMemory(&input, sizeof(input));
	input.ki.dwFlags = KEYEVENTF_KEYUP;
	SendInput(1, &input, sizeof(input));
}
void lockBall()
{
	INPUT input = { 0 };
	input.type = INPUT_KEYBOARD;
	input.ki.wVk = VkKeyScanA(VK_SPACE);
	SendInput(1, &input, sizeof(input));
	ZeroMemory(&input, sizeof(input));
	input.ki.dwFlags = KEYEVENTF_KEYUP;
	SendInput(1, &input, sizeof(input));
}
void jumpPressed()
{
	INPUT input = { 0 };
	input.type = INPUT_KEYBOARD;
	input.ki.wVk = VkKeyScanA(VK_RBUTTON);
	SendInput(1, &input, sizeof(input));
	ZeroMemory(&input, sizeof(input));
	input.ki.dwFlags = KEYEVENTF_KEYUP;
	SendInput(1, &input, sizeof(input));
}
void boostPressed()
{
	INPUT input = { 0 };
	input.type = INPUT_KEYBOARD;
	input.ki.wVk = VkKeyScanA(VK_LBUTTON);
	SendInput(1, &input, sizeof(input));
	ZeroMemory(&input, sizeof(input));
	input.ki.dwFlags = KEYEVENTF_KEYUP;
	SendInput(1, &input, sizeof(input));
}

void dxKeyPressed(const char& c)
{
	SHORT key;
	UINT mappedkey;
	INPUT input = { 0 };
	key = VkKeyScan(c);
	mappedkey = MapVirtualKey(LOBYTE(key), 0);
	input.type = INPUT_KEYBOARD;
	input.ki.dwFlags = KEYEVENTF_SCANCODE;
	input.ki.wScan = mappedkey;
	SendInput(1, &input, sizeof(input));
	Sleep(200);
	input.ki.dwFlags = KEYEVENTF_SCANCODE | KEYEVENTF_KEYUP;
	SendInput(1, &input, sizeof(input));
}
void dxBoost()
{
	SHORT key;
	UINT mappedkey;
	INPUT input = { 0 };
	key = VkKeyScan(VK_LBUTTON);
	mappedkey = MapVirtualKey(LOBYTE(key), 0);
	input.type = INPUT_MOUSE;
	input.ki.dwFlags = KEYEVENTF_SCANCODE;
	input.ki.wScan = mappedkey;
	SendInput(1, &input, sizeof(input));
	Sleep(100);
	input.ki.dwFlags = KEYEVENTF_SCANCODE | KEYEVENTF_KEYUP;
	SendInput(1, &input, sizeof(input));
}
void dxJump()
{
	SHORT key;
	UINT mappedkey;
	INPUT input = { 0 };
	key = VkKeyScan(VK_RBUTTON);
	mappedkey = MapVirtualKey(LOBYTE(key), 0);
	input.type = INPUT_MOUSE;
	input.ki.dwFlags = KEYEVENTF_SCANCODE;
	input.ki.wScan = mappedkey;
	SendInput(1, &input, sizeof(input));
	Sleep(100);
	input.ki.dwFlags = KEYEVENTF_SCANCODE | KEYEVENTF_KEYUP;
	SendInput(1, &input, sizeof(input));
}