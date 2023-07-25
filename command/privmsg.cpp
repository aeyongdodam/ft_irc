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
	message += " ";
	message += clients[fd].getNickName();
	message += " PRIVMSG ";
	message += userNick;
	message += " :";
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
	std::string channelName = str.substr(chennelPoint + 1, spacePoint);

	Channel *channel = server.findChannel(channelName);
	if (channel == NULL)
    {
		numeric = ERR_NOSUCHCHANNEL;
		message += " ";
		message += clients[fd].getNickName();
		message += " ";
		message += userNick;
        message += " :No such channel";
		server.sendMessage(fd, (std::to_string(numeric) + message));
        return ;
    }

	int* clientStatus = channel->getClientStatus(); // 전체 채널 메세지 전송으로 변경예정

	size_t messagePoint = str.find(':');
	std::string chatMessage = str.substr(messagePoint);

	for (int i=0; i<MAX_EVENTS; i++) // 얘도요
	{
		if (clients[i].getRealName() == "")
			continue;
		
		numeric = RPL_AWAY;
		message += " ";
		message += clients[fd].getNickName();
		message += " PRIVMSG #";
		message += channelName;
		message += " :";
		message += chatMessage;
		if (i != fd && clientStatus[i] == CONNECTED)
			server.sendMessage(i, (std::to_string(numeric) + message));
	}
}

void PRIVMSG(int fd, std::string str)
{
	size_t chennelPoint = str.find('#');
	if (chennelPoint == std::string::npos)
		sendUser();
	else
		sendChannel(fd, str, chennelPoint);
}
