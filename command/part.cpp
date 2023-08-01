#include "command.hpp"

std::string	PART(std::string channelName, int clientId)
{
    Server& server = Server::getInstance();
    Channel* channel = server.findChannel(channelName);

    int	responseCode = channel->partClient(clientId);

    if (responseCode == 1)
    {
        Client* clients = server.getClients();
        std::string channelMsg = ":";
        channelMsg += clients[clientId].getNickName();
        channelMsg += " PART :";
        channelMsg += channelName;
        if (channel->isAdmin(clientId) && channel->getAdminIdList().size() - 1 == 0)
        {
            server.deleteChannel(channelName, clientId);
            return "";
        }
        else
        {
            server.sendChannelMessage(channel, channelMsg, clientId);
            std::string resMsg = "PART ";
            resMsg += channelName;
            return resMsg;
        }
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