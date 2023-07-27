#include "command.hpp"
#include "../Server.hpp"
#include "../Channel.hpp"

std::string TOPIC(std::string input, int clientId)
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
        return (std::to_string(numeric) + message);
    }
    std::string channelName = input.substr(0, firstWord);
    Channel *channel = server.findChannel(channelName);
    if (channel == NULL) //채널이 없는 경우
    {
        numeric = ERR_NOSUCHCHANNEL;
        message = " " + clients[clientId].getNickName() + " " + channelName + " :No such channel";
        return (std::to_string(numeric) + message);
    }
    size_t secondWord =  input.find(' ', firstWord + 1);
    if (secondWord == std::string::npos) // 첫 번째 파라미터만 있는 경우
    {
        if (channel->getTopic() == NULL)
        {
            numeric = RPL_NOTOPIC;
            message = " " + channelName + " :No topic is set";
        }
        else
            return (*channel->getTopic());
    }
    std::string topicString = input.substr(firstWord + 2, secondWord - firstWord - 2);
    int *clientStatus = channel->getClientStatus();
    if (clientStatus[clientId] != CONNECTED) // 명령 사용자가 채널에 참여하지 않은 경우
    {
        numeric = ERR_NOTONCHANNEL;
        message = " " + channelName + " :You're not on that channel";
        return (std::to_string(numeric) + message);
    }

    // if (channel->getAdminId() != clientId && channel->gettopicSetting() == 0) //방장 여러 명 배열로 바뀌면 고쳐야함
    // {
    //     numeric = ERR_CHANOPRIVSNEEDED;
    //     message = " " + channelName + " :You're not channel operator";
    //     return (std::to_string(numeric) + message);
    // }
    channel->changeTopic(clientId, topicString);
    std::cout << "channelName " << channelName + " topiccstring : " << topicString << std::endl;
    message = "TOPIC " + channelName + " :" + topicString;
    return (message);
}