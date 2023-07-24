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
		message = " :No nickname given";
	}
	else
	{
		int flag = 1;
		for (int i=0; i<5; i++)
		{
			std::string tempnick = clients[i].getNickName();
			if (nickname == tempnick)
			{
				numeric = ERR_NICKNAMEINUSE;
				message = " " + nickname + " :Nickname is already in use.";
				flag = 0;
				break ;
			}
		}
		if (flag)
		{
			numeric = RPL_WELCOME;
			message = " " + nickname + " :Welcome to the IRC Network !!";
			clients[fd].setNickName(nickname);
		}
	}
	return (std::to_string(numeric) + message);
}