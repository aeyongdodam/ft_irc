#ifndef SERVER_HPP
#define SERVER_HPP

#define MAX_EVENTS 10

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
#include <array>
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
		std::array<std::string, 4> commandList;
		

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
		void commandParsing(std::string input);
		int checkCommand(std::string command);

};

void errProc(const char*);
int checkPassword(char rBuff[BUFSIZ], std::string password, int passflag);

#endif
