#include "command.hpp"

void sendUser(int fd, std::string target, std::string messageStr)
{
	int numeric;
	std::string message;

	Server& server = Server::getInstance();
	Client* clients = server.getClients();

	int nickNameId = server.getNickNameId(target);
	if (nickNameId == -1)
	{
		numeric = ERR_NOSUCHNICK;
		message += " ";
		message += clients[fd].getNickName();
		message += " ";
		message += target;
        message += " :No such nick";
		server.sendMessage(fd, (std::to_string(numeric) + message));
        return ;
	}

	numeric = RPL_AWAY;
	message += ":";
	message += clients[fd].getNickName();
	message += server.prefix(fd);
	message += " PRIVMSG ";
	message += target;
	message += " ";
	message += messageStr;
	
	server.sendMessage(nickNameId, message);
}

void sendChannel(int fd, std::string target, std::string messageStr)
{
	int numeric;
	std::string message;

	Server& server = Server::getInstance();
	Client* clients = server.getClients();
	Channel *channel = server.findChannel(target);
	if (channel == NULL)
    {
		numeric = ERR_NOSUCHCHANNEL;
		message += " ";
		message += clients[fd].getNickName();
		message += " ";
		message += target;
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
		message += target;
        message += " :Cannot send to channel";
		server.sendMessage(fd, (std::to_string(numeric) + message));
        return ;
	}

	message += ":";
	message += clients[fd].getNickName() + server.prefix(fd);
	message += " PRIVMSG ";
	message += target;
	message += " ";
	message += messageStr;
	server.sendChannelMessage(channel, message, fd);
}

void PRIVMSG(int fd, std::string str)
{
	size_t messagePoint = str.find(' ');
	std::string targetsStr = str.substr(0, messagePoint);
	std::list<std::string> targets = split(targetsStr, ',');
	std::string messageStr = str.substr(messagePoint+1);

	while (targets.size() > 0)
    {
        std::string target = targets.front();
        targets.pop_front();
		
		size_t chennelPoint = target.find('#');
		if (chennelPoint == std::string::npos)
			sendUser(fd, target, messageStr);
		else
			sendChannel(fd, target, messageStr);
    }

}
