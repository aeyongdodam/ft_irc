#include "Server.hpp"
#include "Channel.hpp"

Server::Server()
{
	commandList[0] = "PASS";
	commandList[1] = "NICK";
	commandList[2] = "USER";
	commandList[3] = "JOIN";
	commandList[4] = "PRIVMSG";
	commandList[5] = "KICK";
	commandList[6] = "PART";
	commandList[7] = "TOPIC";
	commandList[8] = "MODE";
	commandList[9] = "QUIT";
	commandList[10] = "INVITE";
	connectClientNum = 0;
}

Server::Server(const Server& other)
{
	(void)other;
}

Server& Server::operator=(const Server& source)
{
	(void)source;
	return (*this);
}

Server::~Server(){}

Server& Server::getInstance()
{
	static Server server;

	return server;
}

void Server::init(unsigned short portNum, std::string generalPassword)
{
	this->generalPass = generalPassword;
	for (int i = 0; i < MAX_EVENTS+1; i++) 
	{
		fds[i].fd = -1;
		fds[i].events = 0;
	}

	std::cout << "Server start..." << std::endl;
	listenSd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (listenSd == -1)
		errProc("socket");

	if (fcntl(listenSd, F_SETFL, O_NONBLOCK) == -1)
		errProc("fcntl");

	std::memset(&srvAddr, 0, sizeof(srvAddr));
	srvAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	srvAddr.sin_family = AF_INET;
	srvAddr.sin_port = htons(portNum);

	if (bind(listenSd, (struct sockaddr*)&srvAddr, sizeof(srvAddr)) == -1)
		errProc("bind");

	if (listen(listenSd, MAX_EVENTS+1) < 0)
		errProc("listen");

	fds[0].fd = listenSd;
	fds[0].events = POLLIN;

	clntAddrLen = sizeof(clntAddr);
}

void Server::connectClient(int i)
{
	connectSd = accept(listenSd, (struct sockaddr*)&clntAddr, &clntAddrLen);
	if (connectSd == -1)
	{
		std::cerr << "Accept Error";
		return;
	}

	if (connectClientNum == MAX_EVENTS)
	{
		std::cerr << "MAX_EVENTS limit reached." << std::endl;
		close(connectSd);
		return;
	}

	if (fcntl(connectSd, F_SETFL, O_NONBLOCK) == -1) 
		errProc("fcntl");

	fds[i].fd = connectSd;
	fds[i].events = POLLIN;
	connectClientNum++;
	openNewListenSd();
}

void Server::readClient(int i)
{
	int readfd = fds[i].fd;
	int readLen = read(readfd, rBuff, sizeof(rBuff) - 1);
	if (readLen == 0)
	{
		std::cerr << "A client is disconnected" << std::endl;
		disconnectClient(i, readfd);
	}
	else if (readLen == -1 && errno != EWOULDBLOCK)
	{
		std::cerr << "A client is disconnected (Bad Exit)" << std::endl;
		disconnectClient(i, readfd);
	}
	else if (readLen > 0)
	{
		rBuff[readLen] = '\0';
		std::istringstream iss(rBuff);
		std::string line;
		while (std::getline(iss, line))
		{
			std::cout << "rBuff Message : " << line << std::endl;
			int commandNum = commandParsing(line);
			std::string optionString =  std::strchr(line.c_str(), ' ') + 1;
			optionString.erase(optionString.size() - 1, optionString.size() - 1);
			executeCommand(commandNum, optionString, i);
		}
	}
}

void Server::openNewListenSd()
{
	int flag = 1;
	for (int i=0; i<MAX_EVENTS; i++)
	{
		if (fds[i].fd == -1)
		{
			fds[i].fd = listenSd;
			fds[i].events = POLLIN;
			flag = 0;
			break;
		}
	}
}

void Server::sendMessage(int i, std::string str)
{
	std::string numericMessage = str + "\n";
	std::cout << "메세지내용 : " << numericMessage << std::endl;
	write(fds[i].fd, numericMessage.c_str(), numericMessage.size());
}

const std::string Server::getGenernalPass()
{
	return this->generalPass;
}

Client* Server::getClients()
{
	return this->clients;
}

struct pollfd* Server::getFds()
{
	return fds;
}

void Server::disconnectClient(int i, int readfd)
{
	close(readfd);
	fds[i].fd = -1;
	fds[i].events = 0;
	clients[i].setNickName("");
	clients[i].setLoginName("");
	clients[i].setRealName("");
	clients[i].setPassFlag(false);
	connectClientNum--;

	std::list<Channel*>& channels = clients[i].getChannels();
	while (channels.size() > 0)
	{
		Channel *channel = channels.front();
		channel->getClientStatus()[i] = UNCONNECTED;

		if (channel->isAdmin(i))
		{
			if (channel->getAdminIdList().size() == 1)
				deleteChannel(channel->getName(), i);
			else
				channel->getAdminIdList().remove(i);
		}

		channels.pop_front();
	}
	channels.clear();

	int listenFlag = 1;
	for (int i=0; i<MAX_EVENTS; i++)
		if (fds[i].fd == listenSd)
		{
			listenFlag = 0;
			break;
		}
	if (listenFlag)
		openNewListenSd();
}

void Server::monitoring()
{
	while (true)
	{
		int ready = poll(fds, MAX_EVENTS + 1, -1);
		if (ready == -1)
			errProc("poll");

		for (int i = 0; i < MAX_EVENTS; i++)
		{
			if (fds[i].revents & POLLIN)
			{
				if (fds[i].fd == listenSd)
					connectClient(i);
				else
					readClient(i);
			}
		}
	}
}

void Server::destroy()
{
	close(Server::listenSd);
}

Channel* Server::createChannel(int adminId, std::string &name)
{
	Channel* newChannel = new Channel(adminId, name);
	
	channelMap[name] = newChannel;
	return newChannel;
}

Channel* Server::findChannel(std::string &name)
{
	std::map<std::string, Channel*>::iterator it = channelMap.find(name);
	
	if (it != channelMap.end())
		return it->second;
	else
		return NULL;
}

bool Server::deleteChannel(const std::string &name, int adminId)
{
	std::map<std::string, Channel*>::iterator it = channelMap.find(name);

	// 아직 채널에 남아있는 클라이언트들 킥
	Channel* channel = it->second;
	Server& server = Server::getInstance();
	
	for(int i = 0; i < MAX_EVENTS; i++)
	{
		if (channel->getClientStatus()[i] == CONNECTED)
		{
			std::string kickParameter = channel->getName();
			kickParameter += " ";
			kickParameter += server.getClients()[i].getNickName();
			kickParameter += " ";
			if (adminId != i)
				channel->kickClient(adminId, i);
		}
	}
	channel->kickClient(adminId, adminId);
	if (it != channelMap.end())
	{
		delete it->second;
		it->second = NULL;
		channelMap.erase(it);

		return true;
	}
	return false;
}

void errProc(const char* str)
{
	std::cerr << str << ": " << strerror(errno) << std::endl;
	exit(1);
}

int Server::commandParsing(std::string input)
{
	int commandNum;
	size_t end = input.find(' ');
	if (end != std::string::npos)
	{
		std::string command = input.substr(0, end);
		commandNum = checkCommand(command);
		if (commandNum == -1)
		{
			std::cout << "명령어 없음 " << std::endl;
			return -1;
		}
		return commandNum;
	}
	std::cout << "명령어 없음 " << std::endl;
	return -1;
}

int Server::checkCommand(std::string command)
{
	for (int i = 0; i < CMD_COUNT ; i++)
	{
		if (commandList[i] == command)
			return i;
	}
	std::cout << "그런 명령어는 없어용~" << std::endl;
	return -1;
}

int Server::getNickNameId(std::string kickUserName)
{
	for (int i = 0; i < MAX_EVENTS; i++)
	{
		if (clients[i].getNickName() == kickUserName)
			return i;
	}
	return -1;
}

void Server::executeCommand(int commandNum, std::string optionString, int i)
{
	if (commandNum == 0)
		PASS(optionString, i);
	if (commandNum == 1) // NICK
		sendMessage(i, NICK(i, optionString));
	if (commandNum == 2) // USER
		sendMessage(i, USER(i, optionString));
	if (commandNum == 3) // JOIN
		JOIN(i, optionString);
	if (commandNum == 4) //PRIVMSG
		PRIVMSG(i, optionString);
	if (commandNum == 5) //KICK
		KICK(optionString, i);
	if (commandNum == 6) //PART
		PART(optionString, i);
	if (commandNum == 7) // TOPIC
		sendMessage(i, TOPIC(optionString, i));
	if (commandNum == 8) //MODE
		MODE(i, optionString);
	if (commandNum == 9) //QUIT
		QUIT(i);
	if (commandNum == 10) // INVITE
		INVITE(optionString, i);
}

void Server::sendChannelMessage(Channel *channel, std::string message, int fd)
{
	int* clientStatus = channel->getClientStatus();
	for (int i=0; i<MAX_EVENTS; i++)
	{
		if (clients[i].getRealName() == "")
			continue;
		
		if (i != fd && clientStatus[i] == CONNECTED)
			sendMessage(i, message);
	}
}

void Server::sendChannelUser(int fd, std::string message)
{
	std::vector<int> alreadySent;
	alreadySent.push_back(fd);

	for (std::map<std::string, Channel*>::const_iterator it = channelMap.begin(); it != channelMap.end(); it++)
	{
		Channel* channel = it->second;
		int *clientstatus = channel->getClientStatus();
		if (clientstatus[fd] == CONNECTED)
		{
			for (int i = 0; i < MAX_EVENTS; i++)
			{
				if (clientstatus[i] == CONNECTED && std::find(alreadySent.begin(), alreadySent.end(), i) == alreadySent.end())
				{
					sendMessage(i, message);
					alreadySent.push_back(i);
				}
			}
		}
	}
}
