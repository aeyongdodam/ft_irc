#ifndef COMMAND_HPP
#define COMMAND_HPP

#define SUCCESS 1
#define RPL_WELCOME 1
#define RPL_AWAY 301
#define RPL_NOTOPIC 331
#define RPL_TOPIC 332
#define RPL_INVITING 341
#define RPL_NAMREPLY 353
#define RPL_YOUREOPER 381
#define ERR_NOSUCHNICK 401
#define ERR_NOSUCHSERVER 402
#define ERR_NOSUCHCHANNEL 403
#define ERR_CANNOTSENDTOCHAN 404
#define ERR_TOOMANYCHANNELS 405
#define ERR_NOORIGIN 407
#define ERR_NORECIPIENT 411
#define ERR_NOTEXTTOSEND 412
#define ERR_NOTOPLEVEL 413
#define ERR_WILDTOPLEVEL 414
#define ERR_NONICKNAMEGIVEN 431
#define ERR_ERRONEUSNICKNAME 432
#define ERR_NICKNAMEINUSE 433
#define ERR_NICKCOLLISION 436
#define ERR_USERNOTINCHANNEL 441
#define ERR_NOTONCHANNEL 442
#define ERR_USERONCHANNEL 443
#define ERR_NEEDMOREPARAMS 461
#define ERR_ALREADYREGISTERED 462
#define ERR_PASSWDMISMATCH 464
#define ERR_CHANNELISFULL 471
#define ERR_INVITEONLYCHAN 473
#define ERR_BANNEDFROMCHAN 474
#define ERR_BADCHANNELKEY 475
#define ERR_BADCHANMASK 476
#define ERR_CHANOPRIVSNEEDED 482

#include <iostream>
#include <cstdlib>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <cstring>
#include <unistd.h>
#include <sys/time.h>
#include <poll.h>
#include <cerrno>
#include <fcntl.h>
#include <list>

#include "../Server.hpp"
#include "../Channel.hpp"

class Server;
class Channel;

void PASS(std::string pass, int clientId);
std::string NICK(int fd, std::string nickname);
std::string USER(int fd, std::string str);
void JOIN(int clientId, std::string optionString);
void PRIVMSG(int fd, std::string str);
void KICK(std::string input, int clientId);

std::string	PART(std::string channelName, int clientId);
std::string makePartResponse(int responseCode, std::string channelName);

std::string TOPIC(std::string input, int clientId);

void MODE(int fd, std::string str);
int modeNoChannel(int fd, std::string channelName);
void modeFlagI(int fd, std::string channelName, std::string optionFlag);
void modeFlagT(int fd, std::string channelName, std::string optionFlag);
void modeFlagK(int fd, std::string channelName, std::string optionFlag, std::string textString);
void modeFlagO(int fd, std::string channelName, std::string optionFlag, std::string targetName);
void modeFlagL(int fd, std::string channelName, std::string optionFlag, std::string targetCapacity);

void QUIT(int fd);
#endif