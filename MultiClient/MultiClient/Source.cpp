#pragma comment(lib,"ws2_32.lib") //Required for WinSock
#include <WinSock2.h> //For win sockets
#include <string> //For std::string
#include <iostream> //For std::cout, std::endl, std::cin.getline
#include <WS2tcpip.h>
#include "User.h"
#include <string.h>

SOCKET Connection;//This client's connection to the server

void ClientThread()
{
	char buffer[256]; //Create buffer to hold messages up to 256 characters
	while (true)
	{
		recv(Connection, buffer, sizeof(buffer), NULL); //receive buffer
		std::cout << buffer << std::endl; //print out buffer
	}
	ExitThread(0);
}

int main()
{
	//Winsock Startup
	WSAData wsaData;
	WORD DllVersion = MAKEWORD(2, 1);
	if (WSAStartup(DllVersion, &wsaData) != 0)
	{
		MessageBoxA(NULL, "Winsock startup failed", "Error", MB_OK | MB_ICONERROR);
		return 0;
	}
	std::string ip = "127.0.0.1";
	sockaddr_in addr; //Address to be binded to our Connection socket
	int sizeofaddr = sizeof(addr); //Need sizeofaddr for the connect function
	inet_pton(AF_INET, ip.c_str(), &addr.sin_addr); //Address = localhost (this pc)
	addr.sin_port = htons(1111); //Port = 1111
	addr.sin_family = AF_INET; //IPv4 Socket

	Connection = socket(AF_INET, SOCK_STREAM, 0); //Set Connection socket
	if (connect(Connection, (SOCKADDR*)&addr, sizeof(addr)) != 0) //If we are unable to connect...
	{
		MessageBoxA(NULL, "Failed to Connect", "Error", MB_OK | MB_ICONERROR);
		return 0; //Failed to Connect
	}
	
	User user;
	user.writeUser();
	char temp[256];
	strcpy_s(temp, user.getUsername());
	send(Connection, temp, sizeof(temp), 0);
	if (recv(Connection, temp, sizeof(temp), 0) > 0) {
		std::cout << temp << std::endl;
		if (strcmp(temp, "Connnection successfully") == 0) {
			CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ClientThread, NULL, NULL, NULL);
			char buffer[256]; //256 char buffer to send message
			while (true)
			{
				std::cin.getline(buffer, sizeof(buffer)); //Get line if user presses enter and fill the buffer
				if (strcmp(buffer, "pp") != 0) {
					send(Connection, buffer, sizeof(buffer), NULL); //Send buffer
					Sleep(10);
				}
				else {
					std::cout << "Exitting...." << std::endl;
					char msg[256] = "pp";
					send(Connection, msg, sizeof(msg), 0);
					break;
					exit(1);
				}
			}
		}
	}
	else {
		recv(Connection, temp, sizeof(temp), 0);
		std::cout << temp << std::endl;
	}
	return 0;
}