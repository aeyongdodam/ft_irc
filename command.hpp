#ifndef COMMAND_HPP
#define COMMAND_HPP


#include "Server.hpp"
#include "Client.hpp"
#include "Channel.hpp"

// join.cpp
const std::string join(Server& server, std::string& channelName, int clientId);
const std::string join(Server& server, std::string& channelName, int clientId, std::string& key);
std::string makeJoinResponse(int responseCode);

#endif