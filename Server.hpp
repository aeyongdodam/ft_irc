#ifndef SERVER_HPP
#define SERVER_HPP

#define MAX_EVENTS 10
#define CMD_COUNT 10

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
#include <sstream>

#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#include "Client.hpp"
#include "command/command.hpp"

class Channel;

class Server
{
	private:
		Server();
		Server(const Server& copy);
		~Server();

		struct pollfd fds[MAX_EVENTS + 1];
		struct sockaddr_in srvAddr, clntAddr;
		socklen_t clntAddrLen;
		int listenSd, connectSd;
		char rBuff[BUFSIZ];

		Client clients[MAX_EVENTS  + 1];
		std::map<std::string, Channel*> channelMap;
		std::string commandList[CMD_COUNT];

		int connectClientNum;
		std::string generalPass;

	public:
		Server& operator=(const Server& copy);
		static Server& getInstance();

		void init(unsigned short portNum, std::string generalPassword);
		void monitoring();
		void connectClient(int i);
		void readClient(int i);
		void openNewListenSd();
		void disconnectClient(int i, int readfd);
		void destroy();

		Channel*	createChannel(int adminId, std::string& name);
        Channel*	findChannel(std::string &name);
       	bool 		deleteChannel(const std::string &name, int adminId);

		int commandParsing(std::string input);
		int checkCommand(std::string command);
		int getNickNameId(std::string kickUserName);
		void sendMessage(int i, std::string str);

		const std::string getGenernalPass();
		Client* getClients();
		struct pollfd* getFds();

		void executeCommand(int commandNum, std::string optionString, int i);
		void sendChannelMessage(Channel *channel, std::string message, int fd);
		void sendChannelUser(int fd, std::string message);

		std::string prefix(int fd);
};

void errProc(const char*);

#endif
