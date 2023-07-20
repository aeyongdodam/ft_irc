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
#include "Client.hpp"

class Server
{
	private:
		Server();
		Server(const Server& copy);
		Server& operator=(const Server& copy);

		static std::string nickNames[MAX_EVENTS + 1];
		static struct pollfd fds[MAX_EVENTS + 1];
		static bool passFlag[MAX_EVENTS + 1];
		static struct sockaddr_in srvAddr, clntAddr;
		static socklen_t clntAddrLen;
		static int listenSd, connectSd;
    	static char rBuff[BUFSIZ];

		static Client clients[MAX_EVENTS  + 1];
		

    public:
		~Server();

		static void init(unsigned short portNum);
		static void cennectClient();
		static void readClient(int i, std::string password);
		static void monitoring(std::string password);
		static void destroy();
};

void errProc(const char*);
int checkPassword(char rBuff[BUFSIZ], std::string password, int passflag);

#endif
