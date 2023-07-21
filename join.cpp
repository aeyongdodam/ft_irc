#include "command.hpp"

const std::string join(Server& server, std::string& channelName, int clientId)
{
    int responseCode;
    Channel *channel = server.findChannel(channelName);
    if (channel == NULL)
        channel = server.createChannel(clientId, channelName);

    responseCode = channel->joinChannel(clientId);

    // makejoinResponse(int responseCode) 로 응답 메시지 생성
    // 만들어진 메시지를 해당 채널에 broadcast
}

const std::string join(Server& server, std::string& channelName, int clientId, std::string& key)
{
    Channel *channel = server.findChannel(channelName);

    int responseCode = channel->joinChannel(clientId, key);

    // makeJoinResponse(int responseCode) 로 응답 메시지 생성
    // 만들어진 메시지를 해당 채널에 broadcast
}

std::string makeJoinResponse(int responseCode)
{
    return "Message";
}