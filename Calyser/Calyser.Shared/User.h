#pragma once

using namespace std;

class User
{

private:

	User(){
		//privater contructor
	}

	 string username = "";
	 string ip = "";
	 string device = "";
	 string password = "";
	 long userid = 0L;
	 static User* instance ;


public:
	static User* getInstance();

	string getUsername();

	 string getPassword();

	 long getUserid();

	 void setUsername(string username);

	 void setPassword(string password);

	 void setUserid(long Userid);

	 string getIp();

	 void setIp(string ip);

	 string getDevice();

	 void setDevice(string device);

	~User();
};

