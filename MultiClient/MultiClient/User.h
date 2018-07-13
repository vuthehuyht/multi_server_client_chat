#pragma once
#include <iostream>
#include <string>
using namespace std;
class User
{
public:
	User();
	User(char username_[], char fullname_[], char gender_[], char dateOfBirth_[], char type_[]);
	~User();
	void create();
	char* getUsername();
	char* getFullname();
	char* getGender();
	char* getDateOfBirth();
	char* getType();
	void writeUser();
private:
	char userName[30];
	char fullName[30];
	char gender[5];
	char dateOfBirth[12];
	char type[5];
};

