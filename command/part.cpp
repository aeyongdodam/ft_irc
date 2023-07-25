#include "command.hpp"
#include "../Server.hpp"
#include "../Channel.hpp"

std::string	PART(std::string channelName, int clientId)
{
    Server& server = Server::getInstance();
    Channel* channel = server.findChannel(channelName);

    if (channel->getAdminId() == clientId)
    {
        server.deleteChannel(channelName);
    }

    int	responseCode = channel->partClient(clientId);

    if (responseCode == 1)
    {
        std::string resMsg = "PART :";
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