#ifndef SERVER_HPP
#define SERVER_HPP

#define MAX_EVENTS 10
#define CMD_COUNT 4

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

#include "Client.hpp"
#include "command/command.hpp"

#include <vector>

class Channel;

class Server
{
	private:
		Server(const Server& copy);
		Server& operator=(const Server& copy);

		std::string nickNames[MAX_EVENTS + 1];
		struct pollfd fds[MAX_EVENTS + 1];
		bool passFlag[MAX_EVENTS + 1];
		struct sockaddr_in srvAddr, clntAddr;
		socklen_t clntAddrLen;
		int listenSd, connectSd;
		char rBuff[BUFSIZ];

		Client clients[MAX_EVENTS  + 1];
		std::list<Channel> channels;
		std::string commandList[CMD_COUNT];

	public:
		Server();
		~Server();

		void init(unsigned short portNum);
		void monitoring(std::string password);
		void connectClient(int i);
		void readClient(int i, std::string password);
		void disconnectClient(int i, int readfd);
		void destroy();

		Channel* createChannel(std::string name);
		int commandParsing(std::string input);
		int checkCommand(std::string command);

		void sendMessage(int i, std::string str);

};

void errProc(const char*);

#endif
