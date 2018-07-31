#pragma once
#pragma comment (lib, "ws2_32.lib")
#include <WinSock2.h>
#include <string>
#include <vector>
#include <fstream>
#include <map>
#include "UserInformation.h"

class UserData {
public:
	
	void loadUserData();
	void saveToFile();
	std::vector<UserInformation> getUserData();
	std::vector<std::string> splitArrayOfChar(char message[2048], std::vector<std::string> abc);
	bool checkExistedUser(std::string user);
	void saveNewUser(char message[], SOCKET s);
	void giveModUser(std::string username, std::vector<UserInformation> userData);
private:
	std::fstream f;
	std::vector<UserInformation> userData;
};
