#include "command.hpp"

std::pair<std::string, std::string> splitByFirstSpace(const std::string& input) {
    std::size_t spacePos = input.find(' ');
    
    if (spacePos == std::string::npos) {
        return std::make_pair(input, "");
    } else {
        std::string firstPart = input.substr(0, spacePos);
        std::string secondPart = input.substr(spacePos + 1);
        return std::make_pair(firstPart, secondPart);
    }
}

void PART(std::string optionString, int clientId)
{
    std::pair<std::string, std::string> stringPair = splitByFirstSpace(optionString);

    std::string channelName = stringPair.first;
    std::string partMessage = stringPair.second;

    Server& server = Server::getInstance();
    Channel* channel = server.findChannel(channelName);

    if (channel == NULL)
    {
        std::string resMsg = std::to_string(ERR_NOSUCHCHANNEL) + " ";
        resMsg += channelName;
        resMsg += " :No such channel";
        server.sendMessage(clientId, resMsg);
        return ;
    }

	int	responseCode = channel->partClient(clientId);

    if (responseCode == 1)
    {
        Client* clients = server.getClients();
        std::string channelMsg = ":";
        channelMsg += clients[clientId].getNickName();
        channelMsg += " PART ";
        channelMsg += channelName;
        channelMsg += " ";
        channelMsg += partMessage;
        server.sendMessage(clientId, channelMsg);
        server.sendChannelMessage(channel, channelMsg, clientId);

        if (channel->isAdmin(clientId) && channel->getAdminIdList().size() - 1 == 0)
            server.deleteChannel(channelName, clientId);
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
