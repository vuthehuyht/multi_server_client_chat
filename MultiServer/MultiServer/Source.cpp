#include "UserInformation.h"
#include "UserData.h"
#include "Admin.h"

static std::map<SOCKET, std::string> connection;			//connection list
UserData data;
std::vector<UserInformation> userData;						//user information list
std::vector<std::string> kickOut;							//user list is kicked
std::vector<std::string> keyword = { "/ban", "/unban", "/info", "/mod", "/unmod", "/filter", "/unfilter", "@all" };

std::fstream inforFile;
char msgOK[] = "OK";

// set normal user
void setNormalUser(std::string user) {
	for (std::vector<UserInformation>::iterator i = userData.begin(); i != userData.end(); i++) {
		if (user.compare(i->getUsername()) == 0)
			i->setType(2);
	}
}

// set admin user
void setAdmin(std::string user) {
	for (std::vector<UserInformation>::iterator it = userData.begin(); it != userData.end(); it++) {
		if (strcmp(it->getUsername(), user.c_str()) == 0)
			it->setType(0);
	}
}

//check username
bool checkUser(std::string username) {
	for (int i = 0; i < userData.size(); i++)
		if (username.compare(std::string(userData[i].getUsername())))
			return true;
	return false;
}

std::vector<std::string> splitMessage(char message[]) {
	char* p;
	char* next_token = NULL;
	std::vector<std::string> result;

	p = strtok_s(message, " ", &next_token);
	while (p != NULL) {
		result.push_back(std::string(p));
		p = strtok_s(NULL, " ", &next_token);
	}
	return result;
}

//check keyword in message
bool checkKeyword(std::string text) {
	for (int i = 0; i < keyword.size(); i++) {
		if (text.compare(keyword[i]) == 0)
			return true;
	}
	return false;
}

//check message
void checkMessage(char message[], SOCKET s) {
	char message_temp[1024];
	strcpy_s(message_temp, message);
	std::vector<std::string> splitedMessage = splitMessage(message);
	if (splitedMessage.size() != 0) {
		std::vector<std::string>::iterator first = splitedMessage.begin();
		std::vector<std::string>::iterator second = splitedMessage.begin() + 1;

		if (checkKeyword(first->data())) {
			std::string username = connection[s];
			for (int i = 0; i < userData.size(); i++) {
				if (strcmp(userData[i].getUsername(), username.c_str()) == 0) {
					if (strcmp(userData[i].getType(), "admin") == 0) {
						Admin ad;
						ad.getData(first->data(), s, second->data(), userData);
					}
					else {
						char errMess[] = "You are not admin.";
						send(s, errMess, sizeof(errMess), 0);
					}
				}
			}
		}
		else {
			std::map<SOCKET, std::string>::iterator it;
			std::string username = connection[s];
			for (it = connection.begin(); it != connection.end(); ++it) {
				char buffer_temp[1024];
				ZeroMemory(buffer_temp, sizeof(buffer_temp));
				if (username.compare(it->second) != 0) {
					strcat_s(buffer_temp, username.c_str());
					strcat_s(buffer_temp, ": ");
					strcat_s(buffer_temp, message_temp);
					send(it->first, buffer_temp, sizeof(buffer_temp), 0); // gửi tới các user khác user truyền vào
					std::cout << "Send user " << it->second << ": " << message_temp << std::endl;
				}
			}
		}
	}
	
}

//create thread with user login
void clientHandleThread(SOCKET s) {
	char buffer[1024];
	std::string username = connection[s];
	while (true) {
		ZeroMemory(buffer, sizeof(buffer));
		if (recv(s, buffer, sizeof(buffer), 0) > 0) {
			char buffer_temp_1[1024];
			strcpy_s(buffer_temp_1, buffer);
			char buffer_temp_2[1024];
			strcpy_s(buffer_temp_2, buffer);

			if (strcmp(buffer_temp_1, "pp") != 0) {
				std::cout << "User " << username << ": " << buffer << std::endl;
				checkMessage(buffer_temp_2, s);
			}
			else {
				std::vector<UserInformation>::iterator it = userData.begin();
				if (strcmp(it->getUsername(), username.c_str())) {
					
					connection.erase(s);
					std::cout << username << " exited!" << std::endl;
					std::cout << "Size map after removing: " << connection.size() << std::endl;
					break;
				}
				else {
					connection.erase(s);
					std::cout << username << " exited!" << std::endl;
					std::cout << "Size map after removing: " << connection.size() << std::endl;
					break;
				}
			}
		}
		else {
			std::cout << "Receive fail with error " << WSAGetLastError() << std::endl;
			std::cout << "Size map after removing: ";
			std::cout << connection.size() << std::endl;
			break;
		}
	}
}
int main() {
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
	userData = data.getUserData();
	while (connection.size() < 100) {
		newConnection = accept(sListen, (SOCKADDR*)&addr, &addrlen); //Accept a new connection
		if (newConnection == 0) //If accepting the client connection failed
		{
			std::cout << "Failed to accept the client's connection." << std::endl;
			std::cout << "x2  " << connection.size() << std::endl;
		}
		else {
			char temp[1024];
			ZeroMemory(temp, sizeof(temp));
			recv(newConnection, temp, sizeof(temp), 0); // nhận về thông điệp lựa chọn
			if (strcmp(temp, "1") == 0) {
				send(newConnection, msgOK, sizeof(msgOK), 0);
				recv(newConnection, temp, sizeof(temp), 0);
				if (checkUser(std::string(temp))) {
					std::cout << "Client connnected!" << std::endl;
					char msg[30] = "Connnection successfully";
					send(newConnection, msg, sizeof(msg), 0);
					std::cerr << "Size map before adding " << connection.size() << std::endl;
					connection.insert(std::pair<SOCKET, std::string>(newConnection, std::string(temp)));
					std::cout << "Map size after adding: " << connection.size() << std::endl;
					if (connection.size() == 1) {
						setAdmin(std::string(temp));
						CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)clientHandleThread, (LPVOID)newConnection, NULL, NULL);
					}
					else {
						setNormalUser(std::string(temp));
						CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)clientHandleThread, (LPVOID)newConnection, NULL, NULL);
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
				recv(newConnection, temp, sizeof(temp), 0); 
				if (strlen(temp) > 0) {
					std::cout << temp << std::endl;
					data.saveNewUser(temp, newConnection);
				}
			}
		}
	}
	system("pause");
	return 0;
}
		