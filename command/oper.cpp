#include "command.hpp"

const std::string OPER(std::string& channelName, int clientId)
{
    int responseCode;
    Server& server = Server::getInstance();
    Channel *channel = server.findChannel(channelName);
    if (channel == NULL)
        channel = server.createChannel(clientId, channelName);

    responseCode = channel->joinChannel(clientId);

    return makeJoinResponse(responseCode, channel);
}

const std::string OPER(std::string& channelName, int clientId, std::string& key)
{
    Server& server = Server::getInstance();
    Channel *channel = server.findChannel(channelName);

    int responseCode = channel->joinChannel(clientId, key);

    return makeOPERResponse(responseCode, channel);
}

std::string makeOPERResponse(int responseCode, Channel* channel)
{
    std::string resMsg;

    resMsg += std::to_string(responseCode);
    resMsg += " ";
    resMsg += channel->getName();
    resMsg += " :";

    switch (responseCode)
    {
        case ERR_INVITEONLYCHAN:
            resMsg += "Cannot join channel (+i)";
            break;
        case ERR_BANNEDFROMCHAN:
            resMsg += "Cannot join channel (+b)";
            break;
        case ERR_BADCHANNELKEY:
            resMsg += "Cannot join channel (+k)";
            break;
        case ERR_CHANNELISFULL:
            resMsg += "Cannot join channel (+l)";
            break;
        default:
            std::string* topic = channel->getTopic();
            if (topic)
                resMsg += *topic;
            else
                resMsg += "No topic is set";
    }
    
    return resMsg;
}