#include "command.hpp"
void INVITE(std::string input, int clientId)
{
    Server& server = Server::getInstance();
    std::pair<std::string, std::string> stringPair = splitByFirstSpace(input);
    std::string invitedNickName = stringPair.first;
    std::string channelName = stringPair.second;
    Channel *channel = server.findChannel(channelName);
    Client *clients = server.getClients();
    int *clientStatus = channel->getClientStatus();

    int nickNameId = server.getNickNameId(invitedNickName);
    int numeric;
    std::string message;
    if ((message = checkParameter(invitedNickName, channelName)) != "")
        server.sendMessage(clientId, message);
    else if ((message = checkChannel(channel, channelName)) != "")
        server.sendMessage(clientId, message);
    else if ((message = checkUser(clients, nickNameId)) != "")
        server.sendMessage(clientId, message);
    else if ((message = checkAdminConnected(clientStatus, clientId, channelName)) != "")
        server.sendMessage(clientId, message);
    else if (channel->isAdmin(clientId))
    {
        numeric = RPL_INVITING;
        message = " ";
        message += clients[clientId].getNickName();
        message += " ";
        message += channelName;
        message += " ";
        message += invitedNickName;
        // std::cout << "메세지 내용 : " << message << std::endl;
        server.sendMessage(clientId, std::to_string(numeric) + message);
        message = ":" + clients[clientId].getNickName() + server.prefix(clientId) + " INVITE " + invitedNickName + ":" + channelName;
        server.sendMessage(nickNameId, message);
        channel->inviteClient(clientId, nickNameId);
        return ;
    }
    else //채널 운영자가 아닐 경우
    {
        numeric = ERR_CHANOPRIVSNEEDED;
        message = " ";
        message += channelName;
        message += " :You're not channel operator";
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
        message += " ";
        message += channelName;
        message += " :NO such channel";
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
        message = " ";
        message += clients[nickNameId].getNickName(); 
        message += " :No such nick";
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
        message = " ";
        message += channelName;
        message += " :You're not on that channel";
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
        message = " ";
        message += invitedNickName;
        message += " ";
        message += " :is already on channel";
        return (std::to_string(numeric) + message);
    }
    return "";
}

