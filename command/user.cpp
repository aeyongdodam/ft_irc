#include "command.hpp"

std::string USER(int fd, std::string str)
{
	Server& server = Server::getInstance();
	Client* clients = server.getClients();
	
	int numeric;
	std::string message;

	if (str == "")
	{
		numeric = ERR_NEEDMOREPARAMS;
		message = " USER :Not enough parameters";
	}
	else if (clients[fd].getRealName() != "")
	{
		numeric = ERR_ALREADYREGISTERED;
		message = " :You may not reregister";
	}
	else
	{
		size_t point = str.rfind(' ');
		std::string realname = str.substr(point+2);
		clients[fd].setRealName(realname);
		message = "001 ";
		message += clients[fd].getNickName();
		message +=  " :Welcome to the IRC Network ";
		message += clients[fd].getNickName() + server.prefix(fd);
		return (message);
	}
	return (std::to_string(numeric) + message);
}