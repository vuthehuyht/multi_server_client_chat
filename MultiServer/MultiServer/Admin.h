#pragma once
#include <map>
#include "UserData.h"

class Admin {
public:

	void getData(std::string keyword_, SOCKET s, std::string username, std::vector<UserInformation> detail) {
		if (keyword_.compare("/info") == 0) {				//get data user
			char messGet[10] = "get_info";
			send(s, messGet, sizeof(messGet), 0);
			ZeroMemory(messGet, sizeof(messGet));
			recv(s, messGet, sizeof(messGet), 0);
			if (strcmp(messGet, "OK") == 0) {
				for (int i = 0; i < detail.size(); i++) {
					if (strcmp(detail[i].getUsername(), username.c_str()) == 0) {
						char dataMess[1024];
						ZeroMemory(dataMess, sizeof(dataMess));

						strcat_s(dataMess, "User: ");
						strcat_s(dataMess, detail[i].getUsername());
						strcat_s(dataMess, ",");

						strcat_s(dataMess, "Fullname: ");
						strcat_s(dataMess, detail[i].getFullname());
						strcat_s(dataMess, ",");

						strcat_s(dataMess, "Gender: ");
						strcat_s(dataMess, detail[i].getGender());
						strcat_s(dataMess, ",");

						strcat_s(dataMess, "Birthday: ");
						strcat_s(dataMess, detail[i].getDateOfBirth());
						strcat_s(dataMess, ",");

						strcat_s(dataMess, "Type: ");
						strcat_s(dataMess, detail[i].getType());

						puts(dataMess);
						send(s, dataMess, sizeof(dataMess), 0);
					}
				}
			}
		}
	}

};