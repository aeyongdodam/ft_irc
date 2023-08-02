#include "command.hpp"
void INVITE(std::string input, int clientId)
{
    Server& server = Server::getInstance();
    std::pair<std::string, std::string> stringPair = splitTwoWords(input);
    std::string invitedNickName = stringPair.first;
    std::string channelName = stringPair.second;
    std::cout << "inviteNickName : " << invitedNickName << "channelName : " << channelName;
    Channel *channel = server.findChannel(channelName);
    Client *clients = server.getClients();
    int *clientStatus = channel->getClientStatus();

    int nickNameId = server.getNickNameId(invitedNickName);
    int numeric;
    std::string message;
    if ((message = checkParameter(invitedNickName, channelName)) != "")
    {
        server.sendMessage(clientId, message);
        return;
    }
    if ((message = checkChannel(channel, channelName)) != "")
    {
        server.sendMessage(clientId, message);
        return;
    }
    if ((message = checkUser(clients, nickNameId)) != "")
    {
        server.sendMessage(clientId, message);
        return;
    }
    if ((message = checkAdminConnected(clientStatus, clientId, channelName)) != "")
    {
        server.sendMessage(clientId, message);
        return;
    }
    if ((message = checkAlreadyInChannel(clientStatus, nickNameId, invitedNickName)) != "")
    {
        server.sendMessage(clientId, message);
        return;
    }
    if (channel->isAdmin(clientId))
    {
        channel->inviteClient(clientId, nickNameId);
        numeric = RPL_INVITING;
        message = " " + channelName + " " + invitedNickName;
        server.sendMessage(nickNameId, std::to_string(numeric) + message);
        return ;
    }
    else //채널 운영자가 아닐 경우
    {
        numeric = ERR_CHANOPRIVSNEEDED;
        message = " " + channelName + " :You're not channel operator";
        server.sendMessage(clientId, std::to_string(numeric) + message);
        return ;
    }
}

std::string checkParameter(std::string nickName, std::string channelName)
{
    int numeric;
    std::string message;
    if (nickName == "" || channelName == "")
    {
        numeric = ERR_NEEDMOREPARAMS;
        message = " INVITE :Not enough parameters";
        return (std::to_string(numeric) + message);
    }
    return "";
}

std::string checkChannel(Channel *channel, std::string channelName)
{
    int numeric;
    std::string message;
    if (channel == NULL)
    {
        numeric = ERR_NOSUCHCHANNEL;
        message = " " + channelName + " :NO such channel";
        return (std::to_string(numeric) + message);
    }
    return "";
}

std::string checkUser(Client* clients,int nickNameId)
{
    int numeric;
    std::string message;
    if (nickNameId == -1) //초대하려는 유저가 존재하지 않을때
    {
        numeric = ERR_NOSUCHNICK;
        message = " " + clients[nickNameId].getNickName() + " :No such nick";
        return (std::to_string(numeric) + message);
    }
    return "";
}

std::string checkAdminConnected(int *clientStatus, int clientId, std::string channelName)
{
    int numeric;
    std::string message;
    if (clientStatus[clientId] != CONNECTED) // 명령 사용자가 채널에 참여하지 않은 경우
    {
        numeric = ERR_NOTONCHANNEL;
        message = " " + channelName + " :You're not on that channel";
        return (std::to_string(numeric) + message);
    }
    return "";
}

std::string checkAlreadyInChannel(int *clientStatus, int nickNameId, std::string invitedNickName)
{
    int numeric;
    std::string message;
    if (clientStatus[nickNameId] == CONNECTED)
    {
        numeric = ERR_USERONCHANNEL;
        message = " " + invitedNickName + " " + + " :is already on channel";
        return (std::to_string(numeric) + message);
    }
    return "";
}

