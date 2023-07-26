#include "command.hpp"
#include "../Server.hpp"
#include "../Channel.hpp"

std::list<std::string> split(std::string input, char delimiter) {
    std::list<std::string> channels;
    std::stringstream ss(input);
    std::string temp;
 
    while (getline(ss, temp, delimiter))
        channels.push_back(temp);

    return channels;
}

void splitChannelKey(std::string optionString, std::list<std::string> channelNameList, std::list<std::string> keyList)
{
    std::list<std::string> channel_key = split(optionString, ' ');

    std::string channelStr = channel_key.front();
    channelNameList = split(channelStr, ',');

    std::string keyStr = channel_key.back();
    keyList = split(keyStr, ',');
}

const std::string JOIN(std::string& channelName, int clientId)
{
    int responseCode;
    Server& server = Server::getInstance();
    Channel *channel = server.findChannel(channelName);
    if (channel == NULL)
        channel = server.createChannel(clientId, channelName);

    responseCode = channel->joinChannel(clientId);

    return makeJoinResponse(responseCode, channel);
}

const std::string JOIN(std::string& channelName, int clientId, std::string& key)
{
    Server& server = Server::getInstance();
    Channel *channel = server.findChannel(channelName);

    int responseCode = channel->joinChannel(clientId, key);

    return makeJoinResponse(responseCode, channel);
}

std::string makeJoinResponse(int responseCode, Channel* channel)
{
    std::string resMsg;

    resMsg += std::to_string(responseCode);
    resMsg += " JOIN ";
    resMsg += channel->getName();
    // 리턴 형식이 안 맞아서 주석처리 했어요
    // resMsg += " :";

    // switch (responseCode)
    // {
    //     case ERR_INVITEONLYCHAN:
    //         resMsg += "Cannot join channel (+i)";
    //         break;
    //     case ERR_BANNEDFROMCHAN:
    //         resMsg += "Cannot join channel (+b)";
    //         break;
    //     case ERR_BADCHANNELKEY:
    //         resMsg += "Cannot join channel (+k)";
    //         break;
    //     case ERR_CHANNELISFULL:
    //         resMsg += "Cannot join channel (+l)";
    //         break;
    //     default:
    //         std::string* topic = channel->getTopic();
    //         if (topic)
    //             resMsg += *topic;
    //         else
    //             resMsg += "No topic is set";
    // }
    
    return resMsg;
}