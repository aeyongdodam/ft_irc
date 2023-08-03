#include "command.hpp"

void TOPIC(std::string input, int clientId)
{
    int numeric;
    std::string message;
    Server& server = Server::getInstance();
    Client *clients = server.getClients();
    size_t firstWord = input.find(' ');
    if (firstWord == std::string::npos) //topic 다음 파라미터가 없는 경우
    {
        numeric = ERR_NEEDMOREPARAMS;
        message = " TOPIC :Not enough parameters";
        server.sendMessage(clientId, std::to_string(numeric) + message);
        return ;
    }
    std::string channelName = input.substr(0, firstWord);
    Channel *channel = server.findChannel(channelName);
    if (channel == NULL) //채널이 없는 경우
    {
        numeric = ERR_NOSUCHCHANNEL;
        message = " " + clients[clientId].getNickName() + " " + channelName + " :No such channel";
        server.sendMessage(clientId, std::to_string(numeric) + message);
        return ;
    }
    std::string topicString = input.substr(firstWord + 2);
    if (topicString == "") // 첫 번째 파라미터만 있는 경우
    {
        if (channel->getTopic() == NULL)
        {
            numeric = RPL_NOTOPIC;
            message = " " + channelName + " :No topic is set";
            server.sendMessage(clientId, std::to_string(numeric) + message);
            return ;
        }
        else
            server.sendMessage(clientId, *channel->getTopic());
            return ;
    }
    int *clientStatus = channel->getClientStatus();
    if (clientStatus[clientId] != CONNECTED) // 명령 사용자가 채널에 참여하지 않은 경우
    {
        numeric = ERR_NOTONCHANNEL;
        message = " " + channelName + " :You're not on that channel";
        server.sendMessage(clientId, std::to_string(numeric) + message);
        return ;
    }

    if (channel->isAdmin(clientId) == 0 && channel->gettopicSetting() == 0) //방장 여러 명 배열로 바뀌면 고쳐야함
    {
        numeric = ERR_CHANOPRIVSNEEDED;
        message = " " + channelName + " :You're not channel operator";
        server.sendMessage(clientId, std::to_string(numeric) + message);
        return ;
    }
    channel->changeTopic(clientId, topicString);
    message = ":";
    message += clients[clientId].getNickName() + server.prefix(clientId);
    message += " TOPIC ";
    message += channelName + " :" + topicString;
    server.sendMessage(clientId, message);
    server.sendChannelMessage(channel, message, clientId);
    return ;
}
