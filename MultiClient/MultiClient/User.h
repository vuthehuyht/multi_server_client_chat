#pragma once
#include <iostream>
#include <string>
using namespace std;
class User
{
public:
	User();
	User(char username[], char fullname[], string dateOfBirth_, int age_, char hometown_[]);
	~User();
	void create();
	char* getUsername();
	void writeUser();
private:
	char userName[100];
	char fullName[100];
	string dateOfBirth;
	int age;
	char homeTown[256];
};

