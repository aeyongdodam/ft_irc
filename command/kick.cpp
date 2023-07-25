#include "command.hpp"
#include "../Server.hpp"
#include "../Channel.hpp"

std::string KICK(std::string input, int clientId) //clientId가 쫓아내는입장
{
    int numeric;
    std::string message;

    Server& server = Server::getInstance();
    size_t firstWord = input.find(' ');
    if (firstWord == std::string::npos)
    {
        numeric = ERR_NEEDMOREPARAMS;
        message = "KICK :Not enough parameters";
        return (std::to_string(numeric) + message);
    }
    size_t secondWord =  input.find(' ', firstWord + 1);
    if (secondWord == std::string::npos)
    {
        numeric = ERR_NEEDMOREPARAMS;
        message = "KICK :Not enough parameters";
        return (std::to_string(numeric) + message);     
    }
    std::string channelName = input.substr(0, firstWord);
    std::string kickUserName = input.substr(firstWord + 1, secondWord - firstWord - 1);

    
    Channel *channel = server.findChannel(channelName);
    Client *clients = server.getClients();
    if (channel == NULL)
    {
        numeric = ERR_NOSUCHCHANNEL;
        message = channelName + " :NO such channel";
        return (std::to_string(numeric) + message);
    }
    int nickNameId = server.getNickNameId(kickUserName); //nickNameId가 쫓겨나는 입장
    if (nickNameId == -1)
    {
        numeric = ERR_USERNOTINCHANNEL;
        message = kickUserName + " " + channelName + " :They aren't on that channel";
        return (std::to_string(numeric) + message);
    }
    int *clientStatus = channel->getClientStatus();
    if (clientStatus[clientId] != CONNECTED) // 명령 사용자가 채널에 참여하지 않은 경우
    {
        numeric = ERR_NOTONCHANNEL;
        message = channelName + " :You're not on that channel";
        return (std::to_string(numeric) + message);
    }

    if (channel->getAdminId() == clientId || clients[clientId].getAdminFlag() == true) //방장이거나 서버 관리자인경우
    {
        channel->kickClient(clientId, nickNameId);
        return ("KICK " + channelName + " " + kickUserName);
    }
    else //채널 운영자가 아닐 경우
    {
        numeric = ERR_CHANOPRIVSNEEDED;
        message = channelName + " :You're not channel operator";
        return (std::to_string(numeric) + message);
    }
}
