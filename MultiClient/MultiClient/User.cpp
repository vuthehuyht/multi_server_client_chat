#include "User.h"

User::User(char username[], char fullname[], string dateOfBirth_, int age_, char hometown_[]) {
	strcpy_s(userName, username);
	strcpy_s(fullName, fullname);
	dateOfBirth = dateOfBirth_;
	age = age_;
	strcpy_s(homeTown, hometown_);
}

void User::create() {
	fflush(stdin);
	cout << "Nhap user: ";
	gets_s(userName);
	fflush(stdin);
	cout << "Nhap ho ten: ";
	gets_s(fullName);
	fflush(stdin);
	cout << "Nhap ngay sinh: ";
	getline(cin, dateOfBirth);
	cout << "Nhap tuoi: ";
	cin >> age;
	fflush(stdin);
	cout << "Nhap que quan: ";
	gets_s(homeTown);
}

char* User::getUsername() {
	char* result = userName;
	return result;
}

void User::writeUser() {
	fflush(stdin);
	cout << "Username: ";
	gets_s(userName);
}

User::User()
{
}


User::~User()
{
}
