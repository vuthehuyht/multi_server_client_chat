#pragma once
#include <iostream>
class UserInformation
{
public:
	UserInformation();
	~UserInformation();
	void setUsername(char userName_[30]);
	void setFullName(char fullName_[30]);
	void setGender(char gender_[10]);
	void setBirthday(char dateOfBirth_[12]);
	void setType(int num);

	char* getUsername();
	char* getFullname();
	char* getGender();
	char* getDateOfBirth();
	char* getType();

private:
	char userName[30];
	char fullName[30];
	char gender[10];
	char dateOfBirth[12];
	char type[10];
};

