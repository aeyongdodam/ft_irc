#include "command.hpp"
#include "../Channel.hpp"

void MODE(int fd, std::string str)
{
	size_t spacePoint = str.find(' ');
	std::string channelName = str.substr(0, spacePoint);
	std::string optionFlag = str.substr(spacePoint+1, spacePoint+3);

	if (optionFlag[1] == 'i')
		modeFlagI(fd, channelName, optionFlag);
}

void modeFlagI(int fd, std::string channelName, std::string optionFlag)
{
	int numeric;
	std::string message;

	Server& server = Server::getInstance();
	Channel *channel = server.findChannel(channelName);
	Client* clients = server.getClients();

	if (channel == NULL)
		return ;

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
		server.sendMessage(fd, message);
	}
	else
	{
		message = std::to_string(numeric);
		message += " ";
		message += clients[fd].getNickName();
		message += " ";
		message += channelName;
		message += " :You must have channel op access or above to set channel mode i";
		server.sendMessage(fd, message);
	}
}