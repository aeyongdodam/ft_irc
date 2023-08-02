#include "command.hpp"
#include "../Channel.hpp"

void MODE(int fd, std::string str)
{
	size_t spacePoint = str.find(' ');
	// optionFlag가 없는 경우
	if (spacePoint == std::string::npos)
		return ;
	std::string channelName = str.substr(0, spacePoint);
	std::string optionFlag = str.substr(spacePoint+1, 2);
	size_t spacePoint2 = str.rfind(' ');
	std::string textString = "";
	if (spacePoint != spacePoint2)
		textString = str.substr(spacePoint+4);

	// #channel이 아닐때 무시
	size_t channelPoint = str.find('#');
	if (channelPoint == std::string::npos)
		return ;
	
	// 채널 없을때 403
	if (modeNoChannel(fd, channelName))
		return ;
	
	if (optionFlag[1] == 'i')
		modeFlagI(fd, channelName, optionFlag);
	if (optionFlag[1] == 't')
		modeFlagT(fd, channelName, optionFlag);
	if (optionFlag[1] == 'k')
		modeFlagK(fd, channelName, optionFlag, textString);
	if (optionFlag[1] == 'o')
		modeFlagO(fd, channelName, optionFlag, textString);
	if (optionFlag[1] == 'l')
		modeFlagL(fd, channelName, optionFlag, textString);
}

int modeNoChannel(int fd, std::string channelName)
{
	Server& server = Server::getInstance();
	Channel *channel = server.findChannel(channelName);
	Client* clients = server.getClients();

	if (channel != NULL)
		return 0;

	std::string message = std::to_string(403);
	message += " ";
	message += clients[fd].getNickName();
	message += " ";
	message += channelName;
	message += " :No such channel";
	server.sendMessage(fd, message);

	return 1;
}

void modeFlagI(int fd, std::string channelName, std::string optionFlag)
{
	int numeric;
	std::string message;

	Server& server = Server::getInstance();
	Channel *channel = server.findChannel(channelName);
	Client* clients = server.getClients();

	if (optionFlag[0] == '+')
		numeric = channel->changeInviteOnly(fd,true);
	else
		numeric = channel->changeInviteOnly(fd,false);

	if (numeric == 1)
	{
		message = ":";
		message += clients[fd].getNickName();
		message += " MODE ";
		message += channelName;
		message += " :";
		message += optionFlag;
		server.sendChannelMessage(channel, message, fd);
	}
	else if (numeric == ERR_CHANOPRIVSNEEDED)
	{
		message = std::to_string(numeric);
		message += " ";
		message += clients[fd].getNickName();
		message += " ";
		message += channelName;
		message += " :You must have channel op access";
	}
	else
		return ;
	server.sendMessage(fd, message);
}

void modeFlagT(int fd, std::string channelName, std::string optionFlag)
{
	int numeric;
	std::string message;

	Server& server = Server::getInstance();
	Channel *channel = server.findChannel(channelName);
	Client* clients = server.getClients();

	if (optionFlag[0] == '+')
		numeric = channel->changeTopicSetting(fd, false);
	else
		numeric = channel->changeTopicSetting(fd, true);

	if (numeric == 1)
	{
		message = ":";
		message += clients[fd].getNickName();
		message += " MODE ";
		message += channelName;
		message += " :";
		message += optionFlag;
		server.sendChannelMessage(channel, message, fd);
	}
	else if (numeric == ERR_CHANOPRIVSNEEDED)
	{
		message = std::to_string(numeric);
		message += " ";
		message += clients[fd].getNickName();
		message += " ";
		message += channelName;
		message += " :You must have channel op access";
	}
	else
		return ;
	server.sendMessage(fd, message);
}

void modeFlagK(int fd, std::string channelName, std::string optionFlag, std::string textString)
{
	int numeric;
	std::string message;

	Server& server = Server::getInstance();
	Channel *channel = server.findChannel(channelName);
	Client* clients = server.getClients();

	if (textString == "")
		return ;

	if (optionFlag[0] == '+' && textString != "")
		numeric = channel->changeKey(fd, textString);
	else if (optionFlag[0] == '-')
		numeric = channel->changeKey(fd, "");
	else
		return ;

	if (numeric == 1)
	{
		message = ":";
		message += clients[fd].getNickName();
		message += " MODE ";
		message += channelName;
		message += " :";
		message += optionFlag;
		message += textString;
		server.sendChannelMessage(channel, message, fd);
	}
	else if (numeric == ERR_CHANOPRIVSNEEDED)
	{
		message = std::to_string(numeric);
		message += " ";
		message += clients[fd].getNickName();
		message += " ";
		message += channelName;
		message += " :You must have channel op access";
	}

	server.sendMessage(fd, message);
}

void modeFlagO(int fd, std::string channelName, std::string optionFlag, std::string targetName)
{
	std::string message;

	Server& server = Server::getInstance();
	Channel *channel = server.findChannel(channelName);
	Client* clients = server.getClients();

	if (channel->isAdmin(fd) == false)
	{
		message = std::to_string(ERR_CHANOPRIVSNEEDED);
		message += " ";
		message += clients[fd].getNickName();
		message += " ";
		message += channelName;
		message += " :You must have channel op access";
		server.sendMessage(fd, message);
		return;
	}

	if (targetName == "")
		return ;

	int targetId = server.getNickNameId(targetName);
	if (targetId == - 1) // 없는 닉네임일때
	{
		message = std::to_string(ERR_NOSUCHNICK);
		message += " ";
		message += clients[fd].getNickName();
		message += " ";
		message += targetName;
		message += " :No such nick";
		server.sendMessage(fd, message);
		return ;
	}
	if (channel->getClientStatus()[targetId] != CONNECTED) // 있는 닉네임이나 서버에 없을때
		return;

	bool istargetAdmin = channel->isAdmin(targetId);

	if (istargetAdmin == false && optionFlag[0] == '+')
		channel->addAdmin(fd, targetId);
	else if (istargetAdmin == true && optionFlag[0] == '-')
	{
		if (channel->getAdminIdList().size() - 1 == 0)
		{
			server.deleteChannel(channelName, targetId);
			return ;
		}
		channel->getAdminIdList().remove(targetId);
	}

	message = ":";
	message += clients[fd].getNickName();
	message += " MODE ";
	message += channelName;
	message += " ";
	message += optionFlag;
	message += " :";
	message += targetName;
	server.sendChannelMessage(channel, message, fd);
	server.sendMessage(fd, message);
}

void modeFlagL(int fd, std::string channelName, std::string optionFlag, std::string targetCapacity)
{
	int numeric;
	std::string message;

	Server& server = Server::getInstance();
	Channel *channel = server.findChannel(channelName);
	Client* clients = server.getClients();

	int newCapacity = std::atoi(targetCapacity.c_str()); 

	if (optionFlag[0] == '+')
	{
		if (targetCapacity == "")
			return;
		numeric = channel->changeMaxCapacity(fd, newCapacity);
	}
	else
		numeric = channel->changeMaxCapacity(fd, -1);

	if (numeric == 1)
	{
		message = ":";
		message += clients[fd].getNickName();
		message += " MODE ";
		message += channelName;
		message += " ";
		message += optionFlag;
		message += " :";
		message += std::to_string(newCapacity);
		server.sendChannelMessage(channel, message, fd);
	}
	else if (numeric == ERR_CHANOPRIVSNEEDED)
	{
		message = std::to_string(numeric);
		message += " ";
		message += clients[fd].getNickName();
		message += " ";
		message += channelName;
		message += " :You must have channel op access";
	}
	server.sendMessage(fd, message);
}
