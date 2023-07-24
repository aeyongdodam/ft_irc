#include "command.hpp"

std::string USER(int fd, std::string str, Client clients[])
{
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
        message = " " + clients[fd].getNickName() + " :Welcome to the IRC Network !! [!" + clients[fd].getNickName()  + "@127.0.0.1]";
        size_t point = str.rfind(' ');
		std::string realname = str.substr(point+1);
        clients[fd].setRealName(realname);
    }
	return (std::to_string(numeric) + message);
}