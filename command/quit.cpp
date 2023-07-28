#include "command.hpp"

void QUIT(int fd)
{
    std::string message;
    Server& server = Server::getInstance();
    Client *clients = server.getClients();
    std::map<std::string, Channel*>& channelMap = server.getChannelMap();
    for (std::map<std::string, Channel*>::const_iterator it = channelMap.begin(); it != channelMap.end(); it++) 
    {
        Channel* channel = it->second;
        int *clientstatus = channel->getClientStatus();
        if (clientstatus[fd] == CONNECTED)
        {
            message = ":" + clients[fd].getNickName() + "!user@10.14.2.7 QUIT :leaving\n";
            server.sendChannelMessge(channel,message, fd);
        }
    }
}
