#include "command.hpp"
#include "../Channel.hpp"

std::string MODE(int fd, std::string str)
{
	size_t spacePoint = str.find(' ');
	std::string channelName = str.substr(0, spacePoint);
	std::string optionFlag = str.substr(spacePoint+1, spacePoint+3);

	if (optionFlag[1] == 'i')
		return modeFlagI(fd, channelName, optionFlag);
	return "";
}

std::string modeFlagI(int fd, std::string channelName, std::string optionFlag)
{
	int numeric;
	std::string message;

	Server& server = Server::getInstance();
	Channel *channel = server.findChannel(channelName);
	Client* clients = server.getClients();

	// 젤 처음에 mode <nick> +i 들어오는거 어케 할지 정해야함
	if (channel == NULL)
		return "";

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
	else
	{
		message = std::to_string(numeric);
		message += " ";
		message += clients[fd].getNickName();
		message += " ";
		message += channelName;
		message += " :You must have channel op access or above to set channel mode i";
	}
	return message;
}