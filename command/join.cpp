#include "command.hpp"

std::list<std::string> split(std::string input, char delimiter)
{
    std::list<std::string> channels;
    std::stringstream ss(input);
    std::string temp;

    while (getline(ss, temp, delimiter))
        channels.push_back(temp);

    return channels;
}

void splitChannelKey(std::string optionString, std::list<std::string>& channelNameList, std::list<std::string>& keyList)
{
    std::list<std::string> channel_key = split(optionString, ' ');

    std::string channelStr = channel_key.front();
    channelNameList = split(channelStr, ',');

    if (channel_key.front() != channel_key.back())
    {
        std::string keyStr = channel_key.back();
        keyList = split(keyStr, ',');
    }
}

std::string makeJoinResponse(int responseCode, Channel *channel, int clientId)
{
    Server& server = Server::getInstance();
    Client* client = &(server.getClients()[clientId]);
    std::string clientName = client->getNickName();

    std::string resMsg;

    resMsg += std::to_string(responseCode);
    resMsg += " JOIN ";
    resMsg += channel->getName();

    switch (responseCode)
    {
        case ERR_INVITEONLYCHAN:
            resMsg += " :Cannot join channel (invite only channel)";
            break;
        case ERR_BANNEDFROMCHAN:
            resMsg += " :Cannot join channel (banned from channel)";
            break;
        case ERR_BADCHANNELKEY:
            resMsg += ": Cannot join channel (bad channel key)";
            break;
        case ERR_CHANNELISFULL:
            resMsg += ": Cannot join channel (channel is full)";
            break;
        default:
            client->addChannel(channel);
            
            std::string* topic = channel->getTopic();
            if (topic)
            {
                resMsg += "\r\n332 ";
                resMsg += clientName;
                resMsg += " " + channel->getName();
                resMsg += " :" + *topic;
            }
            resMsg += "\r\n353 ";
            resMsg += clientName;
            resMsg += " = " + channel->getName();
            resMsg += " :" + channel->getClientList();

            resMsg += "\r\n366 ";
            resMsg += clientName;
            resMsg += " " + channel->getName();
            resMsg += " :End of /Names list";
    }

    return resMsg;
}

const std::string joinWithoutKey(std::string &channelName, int clientId)
{
    int responseCode;
    Server &server = Server::getInstance();
    Client client = server.getClients()[clientId];
    Channel *channel = server.findChannel(channelName);
    if (channel == NULL)
        channel = server.createChannel(clientId, channelName);

    responseCode = channel->joinChannel(clientId);

    return makeJoinResponse(responseCode, channel, clientId);
}

const std::string joinWithKey(std::string &channelName, int clientId, std::string &key)
{
    Server &server = Server::getInstance();
    Channel *channel = server.findChannel(channelName);

    std::cout << "joinWithKey(): " << key << std::endl;

    int responseCode = channel->joinChannel(clientId, key);

    return makeJoinResponse(responseCode, channel, clientId);
}

void JOIN(int clientId, std::string optionString)
{
    Server& server = Server::getInstance();

    std::list<std::string> channelNameList;
    std::list<std::string> keyList;
    splitChannelKey(optionString, channelNameList, keyList);

    while (channelNameList.size() > 0)
    {
        std::string channelName = channelNameList.front();
        channelNameList.pop_front();
        if (keyList.size() > 0)
        {
            server.sendMessage(clientId, joinWithKey(channelName, clientId, keyList.front()));
            keyList.pop_front();
        }
        else
            server.sendMessage(clientId, joinWithoutKey(channelName, clientId));
    }
}
