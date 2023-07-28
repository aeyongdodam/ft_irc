#include "command.hpp"
#include "../Channel.hpp"

void MODE(int fd, std::string str)
{
	size_t spacePoint = str.find(' ');
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
		server.sendChannelMessge(channel, message, fd);
	}
	else if (numeric == ERR_CHANOPRIVSNEEDED)
	{
		message = std::to_string(numeric);
		message += " ";
		message += clients[fd].getNickName();
		message += " ";
		message += channelName;
		message += " :You must have channel op access or above to set channel mode i";
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
		server.sendChannelMessge(channel, message, fd);
	}
	else if (numeric == ERR_CHANOPRIVSNEEDED)
	{
		message = std::to_string(numeric);
		message += " ";
		message += clients[fd].getNickName();
		message += " ";
		message += channelName;
		message += " :You must have channel op access or above to set channel mode i";
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
		server.sendChannelMessge(channel, message, fd);
	}
	else if (numeric == ERR_CHANOPRIVSNEEDED)
	{
		message = std::to_string(numeric);
		message += " ";
		message += clients[fd].getNickName();
		message += " ";
		message += channelName;
		message += " :You must have channel op access or above to set channel mode i";
	}
	else
		return ;
	server.sendMessage(fd, message);
}

void 
modeFlagO(int fd, std::string channelName, std::string optionFlag, std::string targetName)
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
		server.sendChannelMessge(channel, message, fd);
		return;
	}

	if (targetName == "")
		return ;

	int targetId = server.getNickNameId(targetName);
	if (targetId == - 1 || channel->getClientStatus()[targetId] != CONNECTED)
	{
		message = targetName;
		message += " ";
		message += channelName; 
		message += " :They aren't on that channel";
		server.sendChannelMessge(channel, message, fd);
		return;
	}

	bool istargetAdmin = channel->isAdmin(targetId);

	if (istargetAdmin == false && optionFlag[0] == '+')
		channel->addAdmin(fd, targetId);
	else if (istargetAdmin == true && optionFlag[0] == '-')
		channel->getAdminIdList().remove(targetId);

	std::string channelClientListStr = channel->getClientList();

	for (int i = 0; i < MAX_EVENTS; i++)
	{
		if (channel->getClientStatus()[i] == CONNECTED)
		{
			message = "353 ";
    		message += clients[i].getNickName();
    		message += " = " + channel->getName();
    		message += " :" + channelClientListStr;

	    	message += "\r\n366 ";
    		message += clients[i].getNickName();
    		message += " " + channel->getName();
    		message += " :End of /Names list";
			server.sendChannelMessge(channel, message, i);
		}
	}
}