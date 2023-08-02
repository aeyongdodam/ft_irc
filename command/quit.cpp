#include "command.hpp"

void QUIT(int fd)
{
    Server& server = Server::getInstance();
    Client *clients = server.getClients();

    std::string message = ":";
    message += clients[fd].getNickName() + server.prefix(fd);
    message += " QUIT :leaving";

    server.sendChannelUser(fd, message);
}
