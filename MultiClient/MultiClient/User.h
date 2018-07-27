#pragma once
#include <iostream>
#include <string>
using namespace std;
class User
{
public:
	User();
	User(char username_[], char fullname_[], char gender_[], char dateOfBirth_[]);
	~User();
	void create();
	char* getUsername();
	char* getFullname();
	char* getGender();
	char* getDateOfBirth();
	void writeUser();
private:
	char userName[31];
	char fullName[30];
	char gender[10];
	char dateOfBirth[12];
};

