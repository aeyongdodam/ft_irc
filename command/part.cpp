#include "command.hpp"

void PART(std::string optionString, int clientId)
{
    std::pair<std::string, std::string> stringPair = splitByFirstSpace(optionString);

    std::string channelName = stringPair.first;
    std::string partMessage = stringPair.second;

    Server& server = Server::getInstance();
    Channel* channel = server.findChannel(channelName);
    Client* clients = server.getClients();

    if (channel == NULL)
    {
        std::string resMsg = std::to_string(ERR_NOSUCHCHANNEL) + " ";
        resMsg += channelName;
        resMsg += " :No such channel";
        server.sendMessage(clientId, resMsg);
        return ;
    }

    if (channel->isAdmin(clientId) && channel->getAdminIdList().size() - 1 == 0)
    {
        server.deleteChannel(channelName, clientId);
        std::string channelMsg = ":";
        channelMsg += clients[clientId].getNickName();
        channelMsg += "! PART :";
        channelMsg += channelName;
        channelMsg += " ";
        channelMsg += partMessage;
        server.sendMessage(clientId, channelMsg);
        return;
    }

	int	responseCode = channel->partClient(clientId);

    if (responseCode == 1)
    {
        std::string channelMsg = ":";
        channelMsg += clients[clientId].getNickName();
        channelMsg += " PART ";
        channelMsg += channelName;
        channelMsg += " ";
        channelMsg += partMessage;
        server.sendMessage(clientId, channelMsg);
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
