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
	
	void loadUserData() {
		std::string data;
		f.open("userdata.txt", std::ios::in);

		if (f.fail())
			std::cout << "Opening file fail" << std::endl;
		else {
			int i = 1;
			while (!f.eof()) {
				getline(f, data);
				UserInformation user;
				if (data.compare("") != 0) {
					char data_temp[100];
					ZeroMemory(data_temp, sizeof(data_temp));
					strcpy_s(data_temp, data.c_str());
					if (i == 1) {
						user.setUsername(data_temp);
						i++;
						continue;
					}
					else if (i == 2) {
						user.setFullName(data_temp);
						i++;
						continue;
					}
					else if (i == 3) {
						user.setGender(data_temp);
						i++;
						continue;
					}
					else if (i == 4) {
						user.setBirthday(data_temp);
						i++;
						if (i == 5) {
							userData.push_back(user);
							i = 1;
						}
					}
				}
				else continue;
			}
			f.close();
		}
	}

	void saveToFile() {
		f.open("userdata.txt", std::ios::out);
		if (f.fail())
			std::cout << "Open file fail" << std::endl;
		else {
			for (std::vector<UserInformation>::iterator i = userData.begin(); i != userData.end(); i++) {
				f << i->getUsername() << std::endl;
				f << i->getFullname() << std::endl;
				f << i->getGender() << std::endl;
				f << i->getDateOfBirth() << std::endl;
			}
		}
		f.close();
	}

	std::vector<UserInformation> getUserData() {
		loadUserData();
		return userData;
	}

	std::vector<std::string> splitArrayOfChar(char message[2048], std::vector<std::string> abc) {
		char* p;
		char* next_token = NULL;
		p = strtok_s(message, ",", &next_token);
		while (p != NULL) {
			abc.push_back(std::string(p));
			p = strtok_s(NULL, ",", &next_token);
		}
		delete[] p, next_token;
		return abc;
	}

	bool checkExistedUser(std::string user) {
		for (std::vector<UserInformation>::iterator i = userData.begin(); i != userData.end(); i++)
			if (user.compare(std::string(i->getUsername())) == 0)
				return true;
		return false;
	}

	void saveNewUser(char message[], SOCKET s) {
		std::vector<std::string> infor = splitArrayOfChar(message, infor); // array of properties user
		UserInformation user;
		int i = 0;
		char data_temp[30];
		ZeroMemory(data_temp, sizeof(data_temp));
		strcpy_s(data_temp, infor[i].c_str());
		if (checkExistedUser(std::string(data_temp))) {
			user.setUsername(data_temp);
			i++; // i = 1
			strcpy_s(data_temp, infor[i].c_str());
			user.setFullName(data_temp);
			i++; //i = 2
			strcpy_s(data_temp, infor[i].c_str());
			user.setGender(data_temp);
			i++; //i = 3
			strcpy_s(data_temp, infor[i].c_str());
			user.setBirthday(data_temp);

			userData.push_back(user);

			std::cout << userData.size() << std::endl;
			saveToFile();
		}
		else {
			char mesCheck[25] = "Username already exist!";
			send(s, mesCheck, sizeof(mesCheck), 0);
		}
	}

	void giveModUser(std::string username, std::vector<UserInformation> userData) {
		for (std::vector<UserInformation>::iterator i = userData.begin(); i != userData.end(); i++) {
			if (username.compare(i->getUsername()) == 0)
				i->setType(1);
		}
	}
private:
	std::fstream f;
	std::vector<UserInformation> userData;
};
