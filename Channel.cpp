#include "Channel.hpp"

Channel::Channel() : adminId(-1), name("default") {}

Channel::Channel(int adminId, std::string& name) : adminId(adminId), name(name), topic(NULL), key(NULL), inviteOnly(false), capacity(1), maxCapacity(-1)
{
	for (int i = 0; i < MAX_EVENTS; i++)
		clientStatus[i] = 0;
	clientStatus[adminId] = CONNECTED;
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
	if (key)
		delete key;
}

int Channel::joinChannel(int clientId)
{
	if (key != NULL)
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
		return 1; // SUCCESS
	case INVITED:
		clientStatus[clientId] = CONNECTED;
		capacity += 1;
		return 1; // SUCCESS
	}

    return false;
}

int Channel::joinChannel(int clientId, std::string& key)
{
	if (this->key->compare(key) != 0)
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

	this->inviteOnly = inviteOnly;
	return 1; // SUCCESS
}

int Channel::changeTopic(int adminId, std::string* topic)
{
	if (this->adminId != adminId)
		return 482; // ERR_CHANOPRIVSNEEDED

	if (this->topic != NULL)
		delete this->topic;

	this->topic = topic;
	return 1; // SUCCESS
}

int Channel::changeKey(int adminId, std::string* key)
{
	if (this->adminId != adminId)
		return 482; // ERR_CHANOPRIVSNEEDED

	if (this->key != NULL)
		delete this->key;

	this->key = key;
	return 1; // SUCCESS
}

int Channel::changeAdmin(int oldAdminId, int newAdminId)
{
	if (this->adminId != oldAdminId)
		return 482; // ERR_CHANOPRIVSNEEDED
	(void)newAdminId;
	adminId = oldAdminId;
	return 1; // SUCCESS
}