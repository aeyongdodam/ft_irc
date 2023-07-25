#include "command.hpp"
#include "../Channel.hpp"

// void sendUser(int fd, std::string str)
// {
// 	int numeric;
// 	std::string message;

// 	Server& server = Server::getInstance();
// 	Client* clients = server.getClients();

// 	// , 단위로 split
// 	// clients돌면서 PRIV보내기
// }

void sendChannel(int fd, std::string str, size_t chennelPoint)
{
	int numeric;
	std::string message;

	Server& server = Server::getInstance();
	Client* clients = server.getClients();

	size_t spacePoint = str.find(' ');
	std::string channelName = str.substr(chennelPoint + 1, spacePoint);

	Channel *channel = server.findChannel(channelName);
	int* clientStatus = channel->getClientStatus();

	size_t messagePoint = str.find(':');
	std::string chatMessage = str.substr(messagePoint);

	for (int i=0; i<MAX_EVENTS; i++)
	{
		if (clients[i].getRealName() == "")
			continue;
		
		numeric = RPL_AWAY;
		message += " PRIVMSG ";
		message += clients[fd].getNickName();
		message += " :";
		message += chatMessage;
		if (i != fd && clientStatus[i] == CONNECTED)
			server.sendMessage(i, (std::to_string(numeric) + message));
	}
}

void PRIVMSG(int fd, std::string str)
{
	size_t chennelPoint = str.find('#');
	// if (chennelPoint == std::string::npos)
	// 	sendUser();
	// else
		sendChannel(fd, str, chennelPoint);
}
