#include "command.hpp"

void PART(std::string optionString, int clientId)
{
    

    Server& server = Server::getInstance();
    Channel* channel = server.findChannel(channelName);
    Client* clients = server.getClients();

    if (channel == NULL)
    {
        std::cout << "==========DEBUG==========" << std::endl;
        std::string resMsg = "403 ";
        resMsg += channelName;
        resMsg += " :No such channel";
        server.sendMessage(clientId, resMsg);
        return ;
    }

    if (channel->isAdmin(clientId))
    {
        channel->getAdminIdList().remove(clientId);
        if (channel->getAdminIdList().size() == 0)
            server.deleteChannel(channelName);
    }

	int	responseCode = channel->partClient(clientId);

    if (responseCode == 1)
    {
        std::string resMsg = ":";
        resMsg += clients[clientId].getNickName();
        resMsg += " PART :";
        resMsg += channelName;
        server.sendMessage(clientId, resMsg);
        
        std::string channelMsg = ":";
        channelMsg += clients[clientId].getNickName();
        channelMsg += " PART ";
        channelMsg += channelName;
        server.sendChannelMessage(channel, channelMsg, clientId);
    }
    else
    {
        std::string resMsg = std::to_string(responseCode);
        resMsg += " ";
        resMsg += channelName;
        if (responseCode == ERR_NOTONCHANNEL)
            resMsg += " :You're not on that channel";
        server.sendMessage(clientId, resMsg);
    }
}
