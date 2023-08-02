#include "command.hpp"

void KICK(std::string input, int clientId) //clientId가 쫓아내는입장, nickNameId가 쫓겨나는 입장
{
    int numeric;
    std::string message;
    std::string channelMessage;
    Server& server = Server::getInstance();

    size_t firstWord = input.find(' ');
    if (firstWord == std::string::npos)
    {
        numeric = ERR_NEEDMOREPARAMS;
        message = " KICK :Not enough parameters";
        server.sendMessage(clientId, std::to_string(numeric) + message);
        return ;
    }
    size_t secondWord =  input.find(' ', firstWord + 1);
    if (secondWord == std::string::npos)
    {
        numeric = ERR_NEEDMOREPARAMS;
        message = " KICK :Not enough parameters";
        server.sendMessage(clientId, std::to_string(numeric) + message); 
        return ; 
    }
    std::string channelName = input.substr(0, firstWord);
    std::string kickUserName = input.substr(firstWord + 1, secondWord - firstWord - 1);
    Channel *channel = server.findChannel(channelName);
    Client *clients = server.getClients();

    if (channel == NULL)
    {
        numeric = ERR_NOSUCHCHANNEL;
        message = " " + channelName + " :NO such channel";
        server.sendMessage(clientId, std::to_string(numeric) + message);
        return ;
    }
    int *clientStatus = channel->getClientStatus();
    int nickNameId = server.getNickNameId(kickUserName); //nickNameId가 쫓겨나는 입장
    if (nickNameId == -1 || clientStatus[nickNameId] != CONNECTED) //쫓겨나야하는 유저가 채널에 없을 때
    {
        numeric = ERR_NOSUCHNICK;
        message = " " + clients[clientId].getNickName() + " " + kickUserName + " :No such nick";
        server.sendMessage(clientId, std::to_string(numeric) + message);
        return ;
    }
    if (clientStatus[clientId] != CONNECTED) // 명령 사용자가 채널에 참여하지 않은 경우
    {
        numeric = ERR_NOTONCHANNEL;
        message = " " + channelName + " :You're not on that channel";
        server.sendMessage(clientId, std::to_string(numeric) + message);
        return ;
    }

    if (channel->isAdmin(clientId))
    {
        if (channel->isAdmin(nickNameId))
        {
            // channel->getAdminIdList().remove(nickNameId);
            if (channel->getAdminIdList().size() - 1 == 0)
            {
                server.deleteChannel(channelName, nickNameId);
            }
            return ;
        }
        channel->kickClient(clientId, nickNameId);
        message = "KICK " + channelName + " " + kickUserName + " " + clients[clientId].getNickName();
        server.sendMessage(clientId, message);
        channelMessage = ":" + clients[clientId].getNickName() + "!user@10.14.2.7 KICK " + channel->getName() + " " + kickUserName + " : ";
        server.sendChannelMessage(channel, channelMessage, clientId);
    }
    else //채널 운영자가 아닐 경우
    {
        numeric = ERR_CHANOPRIVSNEEDED;
        message = " " + channelName + " :You're not channel operator";
        server.sendMessage(clientId, std::to_string(numeric) + message);
        return ;
    }
}