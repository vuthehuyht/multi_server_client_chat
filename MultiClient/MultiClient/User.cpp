#include "User.h"

User::User(char username_[], char fullname_[], char gender_[], char dateOfBirth_[]) {
	strcpy_s(userName, username_);
	strcpy_s(fullName, fullname_);
	strcpy_s(gender, gender_);
	strcpy_s(dateOfBirth, dateOfBirth_);
}

void User::create() {
	cout << "Username: ";
	cin >> userName;
	cout << "Fullname: ";
	fflush(stdin);
	cin >> fullName;
	cout << "Gender: ";
	cin >> gender;
	cout << "Birthday: ";
	cin >> dateOfBirth;
}

char* User::getUsername() {
	char* result = userName;
	return result;
}

char* User::getFullname() {
	char* result = fullName;
	return result;
}

char* User::getGender() {
	char* result = gender;
	return result;
}

char* User::getDateOfBirth() {
	char* result = dateOfBirth;
	return result;
}

void User::writeUser() {
	fflush(stdin);
	cout << "Username: ";
	cin >> userName;
}

User::User()
{
}


User::~User()
{
}
