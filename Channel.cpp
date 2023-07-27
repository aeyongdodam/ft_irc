#include "Channel.hpp"

Channel::Channel() : adminId(-1), name("default") {}

Channel::Channel(int adminId, std::string& name) : adminId(adminId), name(name), topic(NULL), key(""), inviteOnly(false), topicSetting(false), capacity(1), maxCapacity(-1), lastTopicSetId(-1), lastTopicSetTime(-1)
{
	for (int i = 0; i < MAX_EVENTS; i++)
		clientStatus[i] = 0;
	// clientStatus[adminId] = CONNECTED;
	// 첫 채널 생성시 client 462 -> 채널 안 들어가짐
}

Channel::Channel(const Channel& other) : adminId(other.adminId), name(other.name) {}

Channel& Channel::operator=(const Channel& source)
{
	(void)source;
	return (*this);
}

Channel::~Channel()
{
	if (topic)
		delete topic;
	// if (key)
	// 	delete key;
}

int Channel::joinChannel(int clientId)
{
	if (key != "")
		return 475; // ERR_BADCHANNELKEY
	if (capacity == maxCapacity)
		return 471; // ERR_CHANNELISFULL

	int	cStatus = clientStatus[clientId];
	switch (cStatus)
	{
	case BANNED:
		return 474; // ERR_BANNEDFROMCHAN
	case CONNECTED:
		return 462; // SUCCESS
	case UNCONNECTED:
		if (inviteOnly)
			return 473; // ERR_INVITEONLYCHAN
		clientStatus[clientId] = CONNECTED;
		capacity += 1;
		return 311; // SUCCESS
		// 임의로 성공(토픽없음) 리턴
	case INVITED:
		clientStatus[clientId] = CONNECTED;
		capacity += 1;
		return 1; // SUCCESS
	}

    return false;
}

int Channel::joinChannel(int clientId, std::string key)
{
	if (this->key.compare(key) != 0)
		return 475; // ERR_BADCHANNELKEY
	if (capacity == maxCapacity)
		return 471; // ERR_CHANNELISFULL

	int	cStatus = clientStatus[clientId];
	switch (cStatus)
	{
	case BANNED:
		return 474; // ERR_BANNEDFROMCHAN
	case CONNECTED:
		return 1; // SUCCESS
	case UNCONNECTED:
		if (inviteOnly)
			return 473; // ERR_INVITEONLYCHAN
		clientStatus[clientId] = CONNECTED;
		capacity += 1;
		return 1; // SUCCESS
	case INVITED:
		clientStatus[clientId] = CONNECTED;
		capacity += 1;
		return 1; // SUCCESS
	}

    return false;
}

int Channel::kickClient(int adminId, int targetId)
{
	// 관리자가 스스로 강퇴를 하면?
	if (this->adminId != adminId)
		return 482; // ERR_CHANOPRIVSNEEDED

	int	targetStatus = clientStatus[targetId];
	if (targetStatus != CONNECTED)
		return 441; // ERR_USERNOTINCHANNEL
	clientStatus[targetId] = UNCONNECTED;
	capacity -= 1;

	return 1; // SUCCESS
}

int Channel::partClient(int clientId)
{
	if (clientStatus[clientId] != CONNECTED)
		return ERR_NOTONCHANNEL;

	clientStatus[clientId] = UNCONNECTED;
	capacity -= 1;

	return SUCCESS;
}

int Channel::banClient(int adminId, int targetId)
{
	// 관리자가 스스로 밴을 하면?
	if (this->adminId != adminId)
		return 482; // ERR_CHANOPRIVSNEEDED
	if (clientStatus[targetId] == CONNECTED)
		capacity -= 1;
	clientStatus[targetId] = BANNED;

	return 1; // SUCCESS
}

int Channel::inviteClient(int adminId, int targetId)
{
	if (this->adminId != adminId)
		return 482; // ERR_CHANOPRIVSNEEDED

	int	targetStatus = clientStatus[targetId];
	if (targetStatus != CONNECTED)
	{
		clientStatus[targetId] = INVITED;
		return 341; // RPL_INVITING
	}
	return 443; // ERR_USERONCHANNEL
}

int Channel::changeInviteOnly(int adminId, bool inviteOnly)
{
	if (this->adminId != adminId)
		return 482; // ERR_CHANOPRIVSNEEDED
	
	if (this->inviteOnly == inviteOnly)
		return 0; // notihing change

	this->inviteOnly = inviteOnly;
	return 1; // SUCCESS
}

int Channel::changeTopic(int adminId, std::string& topic)
{
	// if (this->adminId != adminId)
	// 	return 482; // ERR_CHANOPRIVSNEEDED

	if (this->topic != NULL)
		*(this->topic) = topic;
	this->lastTopicSetId = adminId; //마지막으로 바꾼 사람 id 저장

	std::time_t timestamp = std::time(0);
	this->lastTopicSetTime = timestamp; // 마지막으로 바꾼 시간 설정
	return 332; // SUCCESS
}

int Channel::changeKey(int adminId, std::string key)
{
	if (this->adminId != adminId)
		return 482; // ERR_CHANOPRIVSNEEDED

	this->key = key;
	return 1; // SUCCESS
}

int Channel::changeAdmin(int oldAdminId, int newAdminId)
{
	if (this->adminId != oldAdminId)
		return 482; // ERR_CHANOPRIVSNEEDED
	adminId = newAdminId;
	return 1; // SUCCESS
}

int Channel::changeTopicSetting(int oldAdminId, bool topicSetting)
{
	if (this->adminId != oldAdminId)
		return 482; // ERR_CHANOPRIVSNEEDED
      
	if (this->topicSetting == topicSetting)
		return 0; // notihing change

	this->topicSetting = topicSetting;
	return 1; // SUCCESS
}

const std::string Channel::getName()
{
    return this->name;
}

std::string* Channel::getTopic()
{
    return this->topic;
}

std::string Channel::getKey()
{
    return this->key;
}

int* Channel::getClientStatus()
{
	return clientStatus;
}

int Channel::getAdminId()
{
	return adminId;
}

bool Channel::gettopicSetting()
{
	return topicSetting;
}

std::string Channel::getClientList()
{
	Server& server = Server::getInstance();

	std::string listStr = "";

    for (int i = 0; i < MAX_EVENTS; i++)
	{
		if (clientStatus[i] == CONNECTED)
		{
			listStr += " ";
			if (i == adminId)
				listStr += "@";
			listStr += server.getClients()[i].getNickName();
		}
	}
	return listStr;
}
