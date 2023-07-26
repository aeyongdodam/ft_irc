#include "command.hpp"
#include "../Channel.hpp"

void sendUser(int fd, std::string str)
{
	int numeric;
	std::string message;

	Server& server = Server::getInstance();
	Client* clients = server.getClients();

	// , 단위로 split
	// clients돌면서 PRIV보내기
	size_t spacePoint = str.find(' ');
	std::string userNick = str.substr(0, spacePoint);

	int nickNameId = server.getNickNameId(userNick);
	if (nickNameId == -1)
	{
		numeric = ERR_NOSUCHNICK;
		message += " ";
		message += clients[fd].getNickName();
		message += " ";
		message += userNick;
        message += " :No such nick";
		server.sendMessage(fd, (std::to_string(numeric) + message));
        return ;
	}

	size_t messagePoint = str.find(':');
	std::string chatMessage = str.substr(messagePoint);

	numeric = RPL_AWAY;
	message += ":";
	message += clients[fd].getNickName();
	message += " PRIVMSG ";
	message += userNick;
	message += " ";
	message += chatMessage;
	
	server.sendMessage(nickNameId, (std::to_string(numeric) + message));
}

void sendChannel(int fd, std::string str, size_t chennelPoint)
{
	int numeric;
	std::string message;

	Server& server = Server::getInstance();
	Client* clients = server.getClients();

	size_t spacePoint = str.find(' ');
	std::string channelName = str.substr(chennelPoint, spacePoint);
	// #포함되어있음. # 빼서 저장된다면 수정해야함

	Channel *channel = server.findChannel(channelName);
	if (channel == NULL)
    {
		numeric = ERR_NOSUCHCHANNEL;
		message += " ";
		message += clients[fd].getNickName();
		message += " ";
		message += channelName;
        message += " :No such channel";
		server.sendMessage(fd, (std::to_string(numeric) + message));
        return ;
    }

	int* clientStatus = channel->getClientStatus();
	if (clientStatus[fd] != CONNECTED)
	{
		numeric = ERR_CANNOTSENDTOCHAN;
		message += " ";
		message += clients[fd].getNickName();
		message += " ";
		message += channelName;
        message += " :Cannot send to channel";
		server.sendMessage(fd, (std::to_string(numeric) + message));
        return ;
	}

	size_t messagePoint = str.find(':');
	std::string chatMessage = str.substr(messagePoint);

	numeric = RPL_AWAY;
	message += ":";
	message += clients[fd].getNickName();
	message += " PRIVMSG ";
	message += channelName;
	message += " ";
	message += chatMessage;
	server.sendChannelMessge(channel, message, fd);
}

void PRIVMSG(int fd, std::string str)
{
	size_t chennelPoint = str.find('#');
	if (chennelPoint == std::string::npos)
		sendUser(fd, str);
	else
		sendChannel(fd, str, chennelPoint);
}
