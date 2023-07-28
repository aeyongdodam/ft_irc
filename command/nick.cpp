#include "command.hpp"

std::string NICK(int fd, std::string nickname)
{
	Server& server = Server::getInstance();
	Client* clients = server.getClients();

	int numeric;
	std::string message;

	if (nickname == "")
	{
		numeric = ERR_NONICKNAMEGIVEN;
		message = std::to_string(numeric);
		message += " :No nickname given";
	}
	else
	{
		int flag = 1;
		for (int i=0; i<MAX_EVENTS; i++)
		{
			std::string tempnick = clients[i].getNickName();
			if (nickname == tempnick)
			{
				if (clients[fd].getNickName() == "")
					clients[fd].setNickName(nickname);
				numeric = ERR_NICKNAMEINUSE;
				message = std::to_string(numeric);
				message += " * " + nickname + " :Nickname is already in use.";
				flag = 0;
				break ;
			}
		}
		if (flag)
		{
			message = ":";
			message += clients[fd].getNickName();
			message += " NICK ";
			message += ":";
			message += nickname;
			clients[fd].setNickName(nickname);
			server.sendChannelUser(fd, message);
		}
	}
	return (message);
}
