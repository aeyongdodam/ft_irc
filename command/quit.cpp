#include "command.hpp"

void QUIT(int fd, std::string str)
{
    (void) fd;
    (void) str;
    std::string message;
    Server& server = Server::getInstance();
    // Client *clients = server.getClients();
    std::map<std::string, Channel*>& channelMap = server.getChannelMap();
    for (std::map<std::string, Channel*>::const_iterator it = channelMap.begin(); it != channelMap.end(); it++) 
    {
        Channel* channel = it->second;
        std::cout << "채널이름 : " << channel->getName() << "-------------- " << std::endl;
    }
}