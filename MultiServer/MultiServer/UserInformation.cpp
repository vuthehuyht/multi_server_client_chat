#include "UserInformation.h"

void UserInformation::setUsername(char userName_[30]) {
	strcpy_s(userName, userName_);
}

void UserInformation::setFullName(char fullName_[30]) {
	strcpy_s(fullName, fullName_);
}

void UserInformation::setBirthday(char dateOfBirth_[12]) {
	strcpy_s(dateOfBirth, dateOfBirth_);
}

void UserInformation::setGender(char gender_[10]) {
	strcpy_s(gender, gender_);
}

char* UserInformation::getUsername() {
	char* result = userName;
	return result;
}

char* UserInformation::getFullname() {
	char* result = fullName;
	return result;
}

char* UserInformation::getGender() {
	char* result = gender;
	return result;
}

char* UserInformation::getDateOfBirth() {
	char* result = dateOfBirth;
	return result;
}

UserInformation::UserInformation()
{
}


UserInformation::~UserInformation()
{
}
