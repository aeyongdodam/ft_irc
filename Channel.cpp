#include "Channel.hpp"

Channel::Channel() : name("default") {}

Channel::Channel(int adminId, std::string& name) : name(name), topic(""), key(""), inviteOnly(false), topicSetting(false), capacity(1), maxCapacity(-1), lastTopicSetId(-1), lastTopicSetTime(-1)
{
	for (int i = 0; i < MAX_EVENTS; i++)
		clientStatus[i] = 0;
	adminIdList.push_back(adminId);
}

Channel::Channel(const Channel& other) : name(other.name) {}

Channel& Channel::operator=(const Channel& source)
{
	(void)source;
	return (*this);
}

Channel::~Channel()
{
}

int Channel::joinChannel(int clientId)
{
	if (key != "")
		return ERR_BADCHANNELKEY;
	if (maxCapacity != -1 && capacity >= maxCapacity)
		return ERR_CHANNELISFULL;

	int	cStatus = clientStatus[clientId];
	switch (cStatus)
	{
	case BANNED:
		return ERR_BANNEDFROMCHAN;
	case CONNECTED:
		return ERR_ALREADYREGISTERED;
	case UNCONNECTED:
		if (inviteOnly)
			return ERR_INVITEONLYCHAN;
		clientStatus[clientId] = CONNECTED;
		capacity += 1;
		return SUCCESS;
	case INVITED:
		clientStatus[clientId] = CONNECTED;
		capacity += 1;
		return SUCCESS;
	}

    return false;
}

int Channel::joinChannel(int clientId, std::string key)
{
	if (!this->key.empty() && this->key.compare(key) != 0)
		return ERR_BADCHANNELKEY;
	if (maxCapacity != -1 && capacity >= maxCapacity)
		return ERR_CHANNELISFULL;

	int	cStatus = clientStatus[clientId];
	switch (cStatus)
	{
	case BANNED:
		return ERR_BANNEDFROMCHAN;
	case CONNECTED:
		return SUCCESS;
	case UNCONNECTED:
		if (inviteOnly)
			return ERR_INVITEONLYCHAN;
		clientStatus[clientId] = CONNECTED;
		capacity += 1;
		return SUCCESS;
	case INVITED:
		clientStatus[clientId] = CONNECTED;
		capacity += 1;
		return SUCCESS;
	}

    return false;
}

int Channel::kickClient(int adminId, int targetId)
{
	Server& server = Server::getInstance();
	Client *clients = server.getClients();

	if (isAdmin(adminId) == false)
		return ERR_CHANOPRIVSNEEDED;

	int	targetStatus = clientStatus[targetId];
	if (targetStatus != CONNECTED)
		return ERR_USERNOTINCHANNEL;
	clientStatus[targetId] = UNCONNECTED;
	capacity -= 1;

	clients[targetId].getChannels().remove(this);

	return SUCCESS;
}

int Channel::partClient(int clientId)
{
	Server& server = Server::getInstance();
	Client *clients = server.getClients();

	if (clientStatus[clientId] != CONNECTED)
		return ERR_NOTONCHANNEL;

	adminIdList.remove(clientId);
	clientStatus[clientId] = UNCONNECTED;
	capacity -= 1;

	clients[clientId].getChannels().remove(this);

	return SUCCESS;
}

int Channel::banClient(int adminId, int targetId)
{
	if (isAdmin(adminId) == false)
		return ERR_CHANOPRIVSNEEDED;

	if (clientStatus[targetId] == CONNECTED)
		capacity -= 1;
	clientStatus[targetId] = BANNED;

	return SUCCESS;
}

int Channel::inviteClient(int adminId, int targetId)
{
	if (isAdmin(adminId) == false)
		return ERR_CHANOPRIVSNEEDED;

	int	targetStatus = clientStatus[targetId];
	if (targetStatus != CONNECTED)
	{
		clientStatus[targetId] = INVITED;
		return RPL_INVITING;
	}
	return ERR_USERONCHANNEL;
}

int Channel::changeInviteOnly(int adminId, bool inviteOnly)
{
	if (isAdmin(adminId) == false)
		return ERR_CHANOPRIVSNEEDED;
	
	if (this->inviteOnly == inviteOnly)
		return 0; // notihing change

	this->inviteOnly = inviteOnly;
	return 1; // SUCCESS
}

int Channel::changeTopic(int adminId, std::string topic)
{
	this->topic = topic;
	this->lastTopicSetId = adminId; //마지막으로 바꾼 사람 id 저장

	std::time_t timestamp = std::time(0);
	this->lastTopicSetTime = timestamp; // 마지막으로 바꾼 시간 설정
	return RPL_TOPIC;
}

int Channel::changeKey(int adminId, std::string key)
{
	if (isAdmin(adminId) == false)
		return ERR_CHANOPRIVSNEEDED;

	if (this->key == "" && key != "") // 새로운 키 등록
		this->key = key;
	else if (this->key != "" && key == "") // 키 삭제
		this->key = key;
	else
		return 0; // 이미 키가 있는데 등록, 키가 없는데 키 삭제
	return SUCCESS;
}

int Channel::changeMaxCapacity(int adminId, int maxCapacity)
{
	if (isAdmin(adminId) == false)
		return ERR_CHANOPRIVSNEEDED;

	this->maxCapacity = maxCapacity;
	return SUCCESS;
}

int Channel::addAdmin(int oldAdminId, int newAdminId)
{
	if (isAdmin(oldAdminId) == false)
		return ERR_CHANOPRIVSNEEDED;

	adminIdList.push_back(newAdminId);
	
	return SUCCESS;
}

int Channel::changeTopicSetting(int adminId, bool topicSetting)
{
	if (isAdmin(adminId) == false)
		return ERR_CHANOPRIVSNEEDED;

	if (this->topicSetting == topicSetting)
		return 0; // notihing change

	this->topicSetting = topicSetting;
	return 1; // SUCCESS
}

const std::string Channel::getName()
{
    return this->name;
}

std::string Channel::getTopic()
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
			if (isAdmin(i))
				listStr += "@";
			listStr += server.getClients()[i].getNickName();
		}
	}
	return listStr;
}

std::list<int>& Channel::getAdminIdList()
{
    return adminIdList;
}

bool Channel::isAdmin(int id)
{
	for(std::list<int>::iterator it = adminIdList.begin(); it != adminIdList.end(); ++it)
	{
		if (*it == id)
			return true;
	}
	return false;
}
