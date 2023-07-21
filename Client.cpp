#include "Client.hpp"


Client::Client()
{
	// channels = NULL;
	passFlag = 0;
}

Client::~Client() {}

std::string Client::getNickName() const
{
	return nickName;
}

std::string Client::getLoginName() const
{
	return loginName;
}

std::string Client::getRealName() const
{
	return realName;
}

bool Client::getPassFlag() const
{
	return passFlag;
}

void Client::setNickName(std::string nickName_)
{
	nickName = nickName_;
}

void Client::setLoginName(std::string loginName_)
{
	loginName = loginName_;
}

void Client::setRealName(std::string setRealName_)
{
	realName = setRealName_;
}

void Client::setPassFlag(bool passFlag_)
{
	passFlag = passFlag_;
}
