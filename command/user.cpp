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
		message +=  " :Welcome to the NyangNyang Chat Service ";
		message += clients[fd].getNickName() + server.prefix(fd);
		message += "\r\n";
		message += "372 " + clients[fd].getNickName() + "\r\n";
		message += "372 " + clients[fd].getNickName() + "                       /^--^\\     /^--^\\     /^--^\\" + "\r\n";
		message += "372 " + clients[fd].getNickName() + "                       \\____/     \\____/     \\____/" + "\r\n";
		message += "372 " + clients[fd].getNickName() + "                      /      \\   /      \\   /      \\" + "\r\n";
		message += "372 " + clients[fd].getNickName() + "                     |        | |        | |        |" + "\r\n";
		message += "372 " + clients[fd].getNickName() + "                      \\__  __/   \\__  __/   \\__  __/" + "\r\n";
		message += "372 " + clients[fd].getNickName() + " |^|^|^|^|^|^|^|^|^|^|^|^\\ \\^|^|^|^/ /^|^|^|^|^\\ \\^|^|^|^|^|^|^|^|^|^|^|^|" + "\r\n";
		message += "372 " + clients[fd].getNickName() + " | | | | | | | | | | | | |\\ \\| | |/ /| | | | | | \\ \\ | | | | | | | | | | |" + "\r\n";
		message += "372 " + clients[fd].getNickName() + " ########################/ /######\\ \\###########/ /#######################" + "\r\n";
		message += "372 " + clients[fd].getNickName() + " | | | | | | | | | | | | \\/| | | | \\/| | | | | |\\/ | | | | | | | | | | | |" + "\r\n";
		message += "372 " + clients[fd].getNickName() + " |_|_|_|_|_|_|_|_|  <<<   seunghso   sohlee   mkwon   >>>  |_|_|_|_|_|_|_|";
		return (message);
	}
	return (std::to_string(numeric) + message);
}
