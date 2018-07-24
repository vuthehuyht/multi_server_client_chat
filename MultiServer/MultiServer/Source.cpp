#pragma comment(lib,"ws2_32.lib")
#include <WinSock2.h>
#include <string>
#include <iostream>
#include <map>
#include <vector>
#include <fstream>
#include "UserInformation.h"

int connected = 0;					//number of client connected

static std::map<SOCKET, std::string> Connections; // connection list

std::vector<std::string> userList;  // user list
std::vector<UserInformation> usersList;  //user information list
std::vector<std::string> kickOutUserList;	// user list is kicked

std::fstream userFile, userInforFile;

char msgOK[3] = "OK";				// OK message
std::string userAdmin;				// admin user


//check amdmin
bool checkAdmin(std::string user) {
	for (std::vector<UserInformation>::iterator i = usersList.begin(); i != usersList.end(); i++) {
		if (strcmp(user.c_str(), i->getUsername()))
			if (strcmp(i->getType(), "admin"))
				return true;
	}
	return false;
}

// set normal user
void setNormalUser(std::string user) {
	for (std::vector<UserInformation>::iterator i = usersList.begin(); i != usersList.end(); i++) {
		if (user.compare(i->getUsername()) == 0)
			i->setType(2);
	}
}

// set admin user
void setAdmin() {
	for (std::vector<UserInformation>::iterator it = usersList.begin(); it != usersList.end(); it++) {
		if (strcmp(it->getUsername(), userAdmin.c_str()) == 0)
			it->setType(0);
	}
}


// split message to check
std::vector<std::string> splitMessage(char message[1024] ) {
	char* p;
	char* next_token;
	std::vector<std::string> result;

	p = strtok_s(message, "., ", &next_token);
	while (p != NULL) {
		result.push_back(std::string(p));
		p = strtok_s(NULL, "., ", &next_token);
	}

	return result;
}


// check message have key word
void checkMessage(char message[1024], SOCKET s) {
	std::string userTemp = Connections[s];

	std::vector<std::string> splited = splitMessage(message);
	std::vector<std::string>::iterator first = splited.begin();
	std::vector<std::string>::iterator second = splited.begin() + 1;

	for (std::vector<UserInformation>::iterator it = usersList.begin(); it != usersList.end(); it++) {
		if (userTemp.compare(it->getUsername()) == 0) {

			if (strcmp("admin", it->getType()) == 0) {

				if (first->compare("/ban") == 0) {
					kickOutUserList.push_back(second->data());
				}
				else if (first->compare("/unban") == 0) {
					kickOutUserList.erase(second);
				}
				else if (first->compare("/info") == 0) {
					char messGet[10] = "get_info";
					send(s, messGet, sizeof(messGet), 0);
					ZeroMemory(messGet, sizeof(messGet));
					recv(s, messGet, sizeof(messGet), 0);
					if (strcmp(messGet, "OK") == 0) {
						for (std::vector<UserInformation>::iterator it = usersList.begin(); it != usersList.end(); it++) {
							if (second->compare(std::string(it->getUsername())) == 0) {
								char inforMess[1024];
								ZeroMemory(inforMess, sizeof(inforMess));
								// nối thông tin để gửi cho user admin
								strcat_s(inforMess, "User: ");
								strcat_s(inforMess, it->getUsername());
								strcat_s(inforMess, ",");

								strcat_s(inforMess, "Fullname: ");
								strcat_s(inforMess, it->getFullname());
								strcat_s(inforMess, ",");

								strcat_s(inforMess, "Gender: ");
								strcat_s(inforMess, it->getGender());
								strcat_s(inforMess, ",");

								strcat_s(inforMess, "Birthday: ");
								strcat_s(inforMess, it->getDateOfBirth());
								strcat_s(inforMess, ",");

								strcat_s(inforMess, "Type: ");
								strcat_s(inforMess, it->getType());

								puts(inforMess);
								send(s, inforMess, sizeof(inforMess), 0);

							}
						}
					}
				}
				else if (first->compare("/mod") == 0) {
					for (std::vector<UserInformation>::iterator it = usersList.begin(); it != usersList.end(); it++) {
						if (second->compare(it->getUsername()) == 0) {
							it->setType(1);
						}
					}
				}
				else if (first->compare("/unmod") == 0) {
					for (std::vector<UserInformation>::iterator it = usersList.begin(); it != usersList.end(); it++) {
						if (second->compare(it->getUsername()) == 0) {
							it->setType(2);
						}
					}
				}
			}
			else {
				if (splited.size() != 2)
					continue;
				else {
					char mesErr[30] = "You are not admin.";
					send(s, mesErr, sizeof(mesErr), 0);
				}
			}
		}
	}
}


//save user information to the file
void saveUserInforToFile() {
	userInforFile.open("userForList.txt", std::ios::out);  // append mode
	if (userInforFile.fail())
		std::cout << "Open fail!" << std::endl;
	else {
		for (std::vector<UserInformation>::iterator it = usersList.begin(); it != usersList.end(); it++) {
			userInforFile << it->getUsername() << std::endl;
			userInforFile << it->getFullname() << std::endl;
			userInforFile << it->getGender() << std::endl;
			userInforFile << it->getDateOfBirth() << std::endl;
		}
		userInforFile.close();
	}
}


//add user information to the list
void loadUserInforToList(std::string file_name) {
	std::fstream f;
	std::string data;
	f.open(file_name, std::ios::in);
	if (f.fail())
		std::cout << "Open fail!" << std::endl;
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
						usersList.push_back(user);
						i = 1;
					}
				}
			}
			else continue;
		}
		f.close();
	}
}

//save users to file
void saveAllUsers() {
	userFile.open("user.txt", std::ios::out);
	for (int i = 0; i < userList.size(); i++)
		userFile << userList[i] << std::endl;
	userFile.close();
}

// remove user from list
void removeList(SOCKET s) {
	std::map<SOCKET, std::string>::iterator it = Connections.find(s);
		Connections.erase(it);
}

//split array of char
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


void initializeUser() {
	userFile.open("user.txt", std::ios::in);
	std::string data;
	while (!userFile.eof()) {
		getline(userFile, data);
		if (data.compare("") != 0)
			userList.push_back(data);
		else continue;
	}
	userFile.close();
}

//check user is in list?
bool checkUser(std::string user) {
	for (std::vector<std::string>::iterator i = userList.begin(); i != userList.end(); i++)
		if (i->compare(user) == 0)
			return true;
	return false;
}

//create thread with user login
void ClientHandlerThread(SOCKET index) {
	char buffer[1024]; //Buffer to receive and send out messages from/to the clients
	std::string userTemp = Connections[index];
	while (true){
		ZeroMemory(buffer, sizeof(buffer));
		std::map<SOCKET, std::string>::iterator it;
		if (recv(index, buffer, sizeof(buffer), 0) > 0) {
			char buffer_temp_1[1024];
			strcpy_s(buffer_temp_1, buffer);
			if (strcmp(buffer_temp_1, "pp") != 0) {
				std::cout << "User " << userTemp << ": " << buffer << std::endl;
				checkMessage(buffer, index);
				for (it = Connections.begin(); it != Connections.end(); ++it) { // duyệt list
					char buffer_temp[1024];
					ZeroMemory(buffer_temp, sizeof(buffer_temp));
					if (userTemp.compare(it->second) != 0) {
						strcat_s(buffer_temp, userTemp.c_str());
						strcat_s(buffer_temp, ": ");
						strcat_s(buffer_temp, buffer);
						send(it->first, buffer_temp, sizeof(buffer_temp), 0); // gửi tới các user khác user truyền vào
						std::cout << "Send user " << it->second << ": " << buffer << std::endl;
					}
				}
			}
			else {
				if (checkAdmin(userTemp)) {
					userAdmin.clear();
					setNormalUser(userTemp);
					std::cout << userTemp << " exited!" << std::endl;
					removeList(index);
					connected--;
					std::cout << "Size map after removing: ";
					std::cout << Connections.size() << endl;
					break;
				}
				else {
					std::cout << userTemp << " exited!" << std::endl;
					removeList(index);
					std::cout << "Size map after removing: ";
					connected--;
					std::cout << Connections.size() << endl;
					break;
				}
			}
		}
		else {
			std::cout << "Receive fail with error " << WSAGetLastError() << std::endl;
			removeList(index);
			connected--;
			std::cout << "Size map after removing: ";
			std::cout << Connections.size() << endl;
			break;
		}
	}
	closesocket(index);
	ExitThread(0);
}

int main(){
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
	initializeUser();  // load username to list
	loadUserInforToList("userForList.txt"); // load user information to the list
	while (connected < 100){
		newConnection = accept(sListen, (SOCKADDR*)&addr, &addrlen); //Accept a new connection
		if (newConnection == 0) //If accepting the client connection failed
		{
			std::cout << "Failed to accept the client's connection." << std::endl;
			std::cout << "x2  " << Connections.size() << std::endl;
		}
		else {
			char temp[1024];
			recv(newConnection, temp, sizeof(temp), 0); // nhận về thông điệp lựa chọn
			if (strcmp(temp, "1") == 0) {				//xác nhận
				send(newConnection, msgOK, sizeof(msgOK), 0);		// gửi đi thông điệp xác nhận thành công
				recv(newConnection, temp, sizeof(temp), 0);
				if (checkUser(std::string(temp))) {
					std::cout << "Client Connected!" << std::endl;
					char msg[30] = "Connnection successfully"; //message sent client to notice connect sucessfully
					send(newConnection, msg, sizeof(msg), 0);
					std::cerr << "Size map before adding " << Connections.size() << std::endl;
					connected++;
					Connections.insert(std::pair<SOCKET, std::string>(newConnection ,std::string(temp)));
					std::cout << "Map size after adding: " << Connections.size() << std::endl;
					if (Connections.size() == 1) {
						userAdmin = std::string(temp);
						setAdmin();
						CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ClientHandlerThread, (LPVOID)newConnection, NULL, NULL);
					}
					else {
						setNormalUser(temp);
						CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ClientHandlerThread, (LPVOID)newConnection, NULL, NULL);
					}
				}
				else {
					char mesErr[50] = "Username incorrect!Check again";
					send(newConnection, mesErr, sizeof(mesErr), 0);
				}
			}
			else if (strcmp(temp, "0") == 0) {
				send(newConnection, msgOK, sizeof(msgOK), 0);
				ZeroMemory(temp, sizeof(temp));
				if (recv(newConnection, temp, sizeof(temp), 0) > 0) {
					std::cout << temp << std::endl;
					std::vector<std::string> infor = splitArrayOfChar(temp, infor); // array of properties user
					UserInformation user;
					int i = 0;
					char data_temp[30];
					ZeroMemory(data_temp, sizeof(data_temp));
					strcpy_s(data_temp, infor[i].c_str());
					if (checkUser(std::string(data_temp))) {
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

						usersList.push_back(user);

						std::cout << usersList.size() << std::endl;
						for (int i = 0; i < usersList.size(); i++)
							std::cout << usersList[i].getUsername() << std::endl;
						saveUserInforToFile();

						userList.push_back(std::string(temp)); //new user is added to the list user
						saveAllUsers(); //save all user to file
					}
					else {
						char mesCheck[25] = "Username already exist!";
						send(newConnection, mesCheck, sizeof(mesCheck), 0);
					}
				}
			}
		}
	}
	system("pause");
	return 0;
}
