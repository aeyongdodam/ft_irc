#include "command.hpp"

std::string USER(int fd, std::string str, Client clients[])
{
	int numeric;
	std::string message;

	if (nickname == "")
	{
		numeric = ERR_NEEDMOREPARAMS;
		message = " USER :Not enough parameters";
	}
	else if (clients[fd].getRealName != "")
	{
		numeric = ERR_ALREADYREGISTERED;
		message = " :You may not reregiste";
	}
    else
    {
        numeric = RPL_WELCOME;
        message = " " + nickname + " :Welcome to the IRC Network !! [!" + nickname + "@127.0.0.1]";
        size_t point = str.rfind(' ');
		std::string realname = str.substr(point, end);
        clients[fd].setRealName(nickname);
    }
	return (std::to_string(numeric) + message);
}