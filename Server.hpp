#ifndef SERVER_HPP
#define SERVER_HPP

#define MAX_EVENTS 10
#define CMD_COUNT 4

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
#include <vector>
#include <map>

#include "Client.hpp"
#include "command/command.hpp"

class Channel;

class Server
{
	private:
		Server();
		Server(const Server& copy);
		~Server();

		std::string nickNames[MAX_EVENTS + 1];
		struct pollfd fds[MAX_EVENTS + 1];
		bool passFlag[MAX_EVENTS + 1];
		struct sockaddr_in srvAddr, clntAddr;
		socklen_t clntAddrLen;
		int listenSd, connectSd;
		char rBuff[BUFSIZ];

		Client clients[MAX_EVENTS  + 1];
		std::map<std::string, Channel*> channelMap;
		std::string commandList[CMD_COUNT];

	public:
		Server& operator=(const Server& copy);
		static Server& getInstance();

		void init(unsigned short portNum);
		void monitoring(std::string password);
		void connectClient(int i);
		void readClient(int i, std::string password);
		void disconnectClient(int i, int readfd);
		void destroy();

		Channel*	createChannel(int adminId, std::string& name);
        Channel*	findChannel(std::string &name);
        bool		deleteChannel(std::string &name);

        int commandParsing(std::string input);
        int checkCommand(std::string command);

		void sendMessage(int i, std::string str);

};

void errProc(const char*);

#endif
