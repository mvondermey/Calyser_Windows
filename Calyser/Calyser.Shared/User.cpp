#include "pch.h"
#include "User.h"


User* User::instance = NULL;



User* User::getInstance() {
	if (! instance) {
		instance = new User();
	}
	return instance;
}


string User::getUsername() {

	return username;
}
string User::getPassword() {

	return password;
}
long User::getUserid() {

	return userid;
}

void User::setUsername(string username) {

	getInstance()->username = username;
}
void User::setPassword(string password) {

	getInstance()->password = password;
}

void User::setUserid(long Userid) {

	getInstance()->userid = Userid;
}

string User::getIp() {

	return ip;
}

void User::setIp(string ip) {

	getInstance()->ip = ip;
}


string User::getDevice() {

	return device;
}

void User::setDevice(string device) {

	getInstance()->device = device;
}



User::~User()
{
}

