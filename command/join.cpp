#include "command.hpp"
#include "../Server.hpp"
#include "../Channel.hpp"

const std::string join(std::string& channelName, int clientId)
{
    int responseCode;
    Server& server = Server::getInstance();
    Channel *channel = server.findChannel(channelName);
    if (channel == NULL)
        channel = server.createChannel(clientId, channelName);

    responseCode = channel->joinChannel(clientId);

    // makejoinResponse(int responseCode) 로 응답 메시지 생성
    // 만들어진 메시지를 해당 채널에 broadcast
}

const std::string join(std::string& channelName, int clientId, std::string& key)
{
    Server& server = Server::getInstance();
    Channel *channel = server.findChannel(channelName);

    int responseCode = channel->joinChannel(clientId, key);

    // makeJoinResponse(int responseCode) 로 응답 메시지 생성
    // 만들어진 메시지를 해당 채널에 broadcast
}

std::string makeJoinResponse(int responseCode)
{
    return "Message";
}