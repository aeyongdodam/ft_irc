#include "command.hpp"
#include "../Channel.hpp"

void PRIVMSG(int fd, std::string str)
{
	Server& server = Server::getInstance();
	Client* clients = server.getClients();

	int numeric;
	std::string message;

	size_t chennelPoint = str.find('#');
	size_t spacePoint = str.find(' ');
	std::string channelName = str.substr(chennelPoint + 1, spacePoint);
	
	Channel *channel = server.findChannel(channelName);
	int* clientStatus = channel->getClientStatus();

	for (int i=0; i<MAX_EVENTS; i++)
	{
		if (clients[i].getRealName() == "")
			continue;
		numeric = 301;
		size_t messagePoint = str.find(':');
		std::string chatMessage = str.substr(messagePoint);
		message = " " + clients[fd].getNickName() + " :" + chatMessage;
		if (fd != i && clientStatus[i] == CONNECTED)
		{
			server.sendMessage(i, message);
			// 세그폴트 발생 가능성 있음
			// 1번 client가 disconnect된 후, 연결 안 된 채로 메세지 보내면 터질 듯 ?
		}
	}
}