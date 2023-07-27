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
        std::cout << "채널이름 : " << channel->getName() << "-------------- " << std::endl;
        if (clientstatus[fd] == CONNECTED)
        {
            message = clients[fd].getNickName() + "!user" + "@127.0.0.1 QUIT :Quit: leaving " + channel->getName();
            server.sendChannelMessge(channel,message, fd);
        }
    }
}
