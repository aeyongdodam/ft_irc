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
		numeric = RPL_WELCOME;
		message = " " + clients[fd].getNickName() + " :Welcome to the IRC Network !!";
		size_t point = str.rfind(' ');
		std::string realname = str.substr(point+1);
		clients[fd].setRealName(realname);
	}
	return (std::to_string(numeric) + message);
}