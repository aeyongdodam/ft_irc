#include "command.hpp"

void QUIT(int fd)
{
    Server& server = Server::getInstance();
    Client *clients = server.getClients();
    std::string message = ":" + clients[fd].getNickName() + "!user@10.14.2.7 QUIT :leaving";
    std::vector<int> alreadySent;
    alreadySent.push_back(fd);
    std::map<std::string, Channel*>& channelMap = server.getChannelMap();
    for (std::map<std::string, Channel*>::const_iterator it = channelMap.begin(); it != channelMap.end(); it++) 
    {
        Channel* channel = it->second;
        int *clientstatus = channel->getClientStatus();
        if (clientstatus[fd] == CONNECTED)
        {
            for (int i = 0; i < MAX_EVENTS; i++)
            {
                if (clientstatus[i] == CONNECTED && std::find(alreadySent.begin(), alreadySent.end(), i) == alreadySent.end()) // 안에 없을경우
                {
                    server.sendMessage(i, message);
                    alreadySent.push_back(i);
                }
            }

        }
    }
}
