#include "Server.hpp"
#include "Channel.hpp"

Server::Server()
{
	commandList[0] = "PASS";
	commandList[1] = "NICK";
	commandList[2] = "USER";
	commandList[3] = "JOIN";
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

void Server::init(unsigned short portNum)
{
	for (int i = 0; i < MAX_EVENTS + 1; i++) 
	{
		fds[i].fd = -1;
		fds[i].events = 0;
		passFlag[i] = 0;
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

	if (listen(listenSd, 5) < 0)
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
	if (fcntl(connectSd, F_SETFL, O_NONBLOCK) == -1) 
		errProc("fcntl");

	fds[i].fd = connectSd;
	fds[i].events = POLLIN;
}

void Server::readClient(int i, std::string password)
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
		std::cout << "rBuff Message : " << rBuff << std::endl;
		// rBuff 파싱
		int commandNum = commandParsing(rBuff);
		std::string optionString =  std::strchr(rBuff, ' ') + 1;
		optionString.erase(optionString.size() - 2, optionString.size() - 1);
		if (commandNum == 0)
			checkPassword(optionString, password);

    // 채널 운영자가 운영자를 강퇴 -> 쿠테타, 그냥 강퇴시키자 -> targetId clientStatus[]
    // 운영자가 채널 운영자 강퇴 ->
    // 강퇴가 안 된다 (방이 안 터져야 되면) -> 쿠데타
    // 
		// sendMeddage 함수 사용 예시
		if (commandNum == 1) //USER NICK
		{
			sendMessage(i, NICK(i, optionString, clients));
		}
		// if (commandNum == 3) // JOIN
		// {
		//     std::string str = std::to_string(331) + " channelName :No topic is set";
		//     sendMessage(i, str);
		// }
	}
}

void Server::sendMessage(int i, std::string str)
{
    std::string numericMessage = ":127.0.0.1 " + str + "\r\n";
    write(fds[i].fd, numericMessage.c_str(), numericMessage.size());
}

void Server::disconnectClient(int i, int readfd)
{
	close(readfd);
	fds[i].fd = -1;
	fds[i].events = 0;
	passFlag[i] = 0;
}

void Server::monitoring(std::string password)
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
					readClient(i, password);
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

bool Server::deleteChannel(std::string &name)
{
	std::map<std::string, Channel*>::iterator it = channelMap.find(name);
	
	if (it != channelMap.end())
	{
		delete it->second;
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