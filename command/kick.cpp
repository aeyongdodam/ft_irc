#include "command.hpp"
#include "../Server.hpp"
#include "../Channel.hpp"

std::string KICK(std::string input, int clientId)
{
    int numeric;
    std::string message;

    Server& server = Server::getInstance();
    size_t firstWord = input.find(' ');
    size_t secondWord =  input.find(' ', firstWord + 1);
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
    int nickNameId = getNickNameId(clients, kickUserName);
    if (nickNameId == -1)
    {
        numeric = ERR_USERNOTINCHANNEL;
        message = kickUserName + " " + channel + " :They aren't on that channel";
        return (std::to_string(numeric) + message);
    }

}

int getNickNameId(Client *clients, std::string kickUserName)
{
    for (int i = 0; i < MAX_EVENTS; i++)
    {
        if (clients[i].getNickName() == kickUserName)
            return i;
    }
    return -1;
}