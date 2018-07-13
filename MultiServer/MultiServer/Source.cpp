#pragma comment(lib,"ws2_32.lib")
#include <WinSock2.h>
#include <string>
#include <iostream>
#include <map>
#include <vector>
#include <fstream>
#include <stdlib.h>
std::map<std::string, SOCKET> Connections;
std::vector<std::string> userList;
std::fstream userFile, userInforFile;

//save users to file
void saveAllUsers() {
	userFile.open("user.txt", std::ios::out);
	for (int i = 0; i < userList.size(); i++)
		userFile << userList[i] << std::endl;
	userFile.close();
}

// remove user from list
void removeList(std::string user) { 
	std::map<std::string, SOCKET>::iterator it = Connections.find(user);
	if (it != Connections.end())
		Connections.erase(it);
}

//split array of char
std::vector<std::string> splitArrayOfChar(char message[2048], std::vector<std::string> abc) {
	char* p;
	int index = 0;
	char* next_token = NULL;
	p = strtok_s(message, ",", &next_token);
	while (p != NULL) {
		abc.push_back(std::string(p));
		p = strtok_s(NULL, ",", &next_token);
	}
	return abc;
}


void initializeUser() {
	userFile.open("user.txt", std::ios::in);
	std::string data;
	while (!userFile.eof()) {
		getline(userFile, data);
		userList.push_back(data);
	}
	userFile.close();
}

bool checkUser(std::string user) {
	for (std::vector<std::string>::iterator i = userList.begin(); i != userList.end(); i++)
		if (i->compare(user) == 0)
			return true;
	return false;
}

void ClientHandlerThread(char* user) //index = the index in the SOCKET Connections array
{
	char buffer[256]; //Buffer to receive and send out messages from/to the clients
	std::string userTemp = user;
	while (true)
	{
		ZeroMemory(buffer, sizeof(buffer));
		std::map<std::string, SOCKET>::iterator it;
		if (recv(Connections[userTemp], buffer, sizeof(buffer), 0) > 0) {
			if (strcmp(buffer, "pp") != 0) {
				std::cout << "user:" << userTemp << ": " << buffer << std::endl;
				for (it = Connections.begin(); it != Connections.end(); ++it) { // duyệt list
					if (userTemp.compare(it->first) != 0) {
						send(it->second, buffer, sizeof(buffer), 0); // gửi tới các user khác user truyền vào
						std::cout << "Send user: " << it->first << ": " << buffer << std::endl;
					}
				}
			}
			else {
				std::cout << userTemp << " exited!" << std::endl;
				removeList(userTemp);
				break;
			}
		}
		else {
			std::cout << "Receive fail with error " << WSAGetLastError() << std::endl;
			removeList(userTemp);
			break;
		}
	}
	closesocket(Connections[userTemp]);
	ExitThread(0);
}

int main()
{
	//Winsock Startup
	WSAData wsaData;
	WORD DllVersion = MAKEWORD(2, 2);
	if (WSAStartup(DllVersion, &wsaData) != 0) //If WSAStartup returns anything other than 0, then that means an error has occured in the WinSock Startup.
	{
		MessageBoxA(NULL, "WinSock startup failed", "Error", MB_OK | MB_ICONERROR);
		return 0;
	}

	SOCKADDR_IN addr; //Address that we will bind our listening socket to
	int addrlen = sizeof(addr); //length of the address (required for accept call)
	addr.sin_addr.S_un.S_addr = INADDR_ANY; //Broadcast locally
	addr.sin_port = htons(1111); //Port
	addr.sin_family = AF_INET; //IPv4 Socket

	SOCKET sListen = socket(AF_INET, SOCK_STREAM, 0); //Create socket to listen for new connections
	bind(sListen, (SOCKADDR*)&addr, addrlen); //Bind the address to the socket
	listen(sListen, SOMAXCONN); //Places sListen socket in a state in which it is listening for an incoming connection. Note:SOMAXCONN = Socket Oustanding Max Connections

	SOCKET newConnection; //Socket to hold the client's connection
	initializeUser();
	for (int i = 0; i < 100; i++)
	{
		newConnection = accept(sListen, (SOCKADDR*)&addr, &addrlen); //Accept a new connection
		if (newConnection == 0) //If accepting the client connection failed
		{
			std::cout << "Failed to accept the client's connection." << std::endl;
		}
		else {
			char temp[2048];
			recv(newConnection, temp, sizeof(temp), 0);
			std::cout << temp << std::endl;
			if (checkUser(std::string(temp))) {
				std::cout << "Client Connected!" << std::endl;
				char msg[256] = "Connnection successfully";
				send(newConnection, msg, sizeof(msg), 0);
				Connections.insert(std::make_pair(std::string(temp), newConnection));
				CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ClientHandlerThread, (LPVOID)temp, NULL, NULL);
			}
			else {
				userList.push_back(std::string(temp));
				saveAllUsers();
				char msg[256] = "none";
				send(newConnection, msg, sizeof(msg), 0);
				if (recv(newConnection, temp, sizeof(temp), 0) > 0)
					std::cout << temp << std::endl;
				std::vector<std::string> infor = splitArrayOfChar(temp, infor);
				userInforFile.open("userForList.txt", std::ios::out);
				for (int i = 0; i < infor.size(); i++) {
					std::cout << infor[i] << " " << std::endl;
					userInforFile << infor[i] << std::endl;
				}
				userInforFile.close();
			}
		}
	}
	system("pause");
	return 0;
}