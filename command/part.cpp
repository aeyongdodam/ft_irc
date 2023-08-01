#include "command.hpp"

void PART(std::string channelName, int clientId)
{
    Server& server = Server::getInstance();
    Channel* channel = server.findChannel(channelName);
    Client* clients = server.getClients();

    int	responseCode = channel->partClient(clientId);

    if (responseCode == 1)
    {
        // std::string resMsg = ":";
        // resMsg += clients[clientId].getNickName();
        // resMsg += " PART ";
        // resMsg += channelName;
        std::string resMsg = "PART ";
        resMsg += channel->getName();
        server.sendMessage(clientId, resMsg);
        
        std::string channelMsg = ":";
        channelMsg += clients[clientId].getNickName();
        channelMsg += " PART ";
        channelMsg += channelName;
        server.sendChannelMessage(channel, channelMsg, clientId);

        std::string resMsg = "PART ";
        resMsg += channelName;
        return resMsg;
    }

	return makePartResponse(responseCode, channelName);
}

std::string makePartResponse(int responseCode, std::string channelName)
{
    std::string resMsg = std::to_string(responseCode);
    resMsg += " ";
    resMsg += channelName;
    if (responseCode == ERR_NOTONCHANNEL)
        resMsg += " :You're not on that channel";
    if (responseCode == ERR_NOTONCHANNEL)
        resMsg += " :No such channel";
        
    return resMsg;
}
