#include "command.hpp"
void INVITE(std::string input, int clientId)
{
    Server& server = Server::getInstance();
    Channel *channel = server.findChannel(channelName);
    Client *clients = server.getClients();
    int *clientStatus = channel->getClientStatus();
    std::pair<std::string, std::string> stringPair = splitTwoWords(input);
    std::string invitedNickName = stringPair.first;
    std::string channelName = stringPair.second;
    int nickNameId = server.getNickNameId(invitedNickName);
    std::string message;
    if ((message = checkParameter(nickName, channelName)) != "")
    {
        server.sendMessage(clientId, message);
        return;
    }
    if ((message = checkChannel(channel)) != "")
    {
        server.sendMessage(clientId, message);
        return;
    }
    if ((message = checkUser(nickNameId)) != "")
    {
        server.sendMessage(clientId, message);
        return;
    }
    if ((message = checkAdminConnected(clientStatus, clientId, invitedNickName)) != "")
    {
        server.sendMessage(clientId, message);
        return;
    }
    if (channel->isAdmin(clientId))
    {
        inviteClient(clientId, nickNameId);
        numeric = RPL_INVITING;
        message = " " + chanelName + " " + invitedNickName;
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

std::string checkParmeter(std::string nickName, std::string channelName)
{
    int numeric;
    std::string message;
    if (nickName == "" || channleName == "")
    {
        numeric = ERR_NEEDMOREPARAMS;
        message = " INVITE :Not enough parameters";
        return (std::to_string(numeric) + message);
    }
    return "";
}

std::string checkChannel(Channel *channel)
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

std::string checkUser(int nickNameId)
{
    int numeric;
    std::string message;
    if (nickNameId == -1) //초대하려는 유저가 존재하지 않을때
    {
        numeric = ERR_NOSUCHNICK;
        message = " " + clients[clientId].getNickName() + " " + kickUserName + " :No such nick";
        return (std::to_string(numeric) + message);
    }
    return "";
}

std::string checkAdminConnected(int *clientStatus, int clientId)
{
    if (clientStatus[clientId] != CONNECTED) // 명령 사용자가 채널에 참여하지 않은 경우
    {
        numeric = ERR_NOTONCHANNEL;
        message = " " + channelName + " :You're not on that channel";
        return (std::to_string(numeric) + message);
    }
    return "";
}

std::string checkAlreadyInChannel(int *clientStatus, int nickNameId, int invitedNickName)
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

