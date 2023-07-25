#include "command.hpp"

std::string TOPIC(std::string input, int clientId)
{
    int numeric;
    std::string message;
    Server& server = Server::getInstance();
    Client *clients = server.getClients();
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
}