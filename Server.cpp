#include "Server.hpp"

struct pollfd Server::fds[MAX_EVENTS + 1];
struct sockaddr_in Server::srvAddr, Server::clntAddr;
int Server::ready, Server::readfd;
int Server::listenSd, Server::connectSd;
int Server::readLen;
std::string Server::nickNames[MAX_EVENTS + 1];
socklen_t Server::clntAddrLen;
char Server::rBuff[BUFSIZ];

Server::Server() {
}

Server::Server(const Server& other) {
    (void)other;
}

Server& Server::operator=(const Server& source) {
	(void)source;
	return (*this);
}

Server::~Server() {
}

void Server::init(unsigned short portNum) {
    for (int i = 0; i < MAX_EVENTS + 1; i++) {
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

    if (listen(listenSd, 5) < 0)
        errProc("listen");

	fds[0].fd = listenSd;
	fds[0].events = POLLIN;

	clntAddrLen = sizeof(clntAddr);
}

void Server::monitoring() {
	while (true) {
		std::cout << "Monitoring ..." << std::endl;
        ready = poll(fds, MAX_EVENTS + 1, -1);
        if (ready == -1)
        {
            if (errno == EINTR)
                continue;
            else
                errProc("poll");
        }
		int i = 0;
		for (i = 0; i < MAX_EVENTS; i++)
        {
            if (fds[i].revents & POLLIN)
            {
                if (fds[i].fd == listenSd)
                { // accept a client
                    connectSd = accept(listenSd, (struct sockaddr*)&clntAddr, &clntAddrLen);
                    if (connectSd == -1)
                    {
                        std::cerr << "Accept Error";
                        continue;
                    }
                    std::cerr << "A client is connected..." << std::endl;
                    
                    // Set the new client socket to non-blocking mode
                    if (fcntl(connectSd, F_SETFL, O_NONBLOCK) == -1)
                        errProc("fcntl");
                    
                    // Find an empty slot in the fds array
                    int k = 0;
                    for (int j = 1; j < MAX_EVENTS + 1; j++) {
                        if (fds[j].fd == -1) {
                            fds[j].fd = connectSd;
                            fds[j].events = POLLIN;
                            k = j;
                            break;
                        }
                    }
                    
                    write(fds[k].fd, "닉네임을 입력해주세요:", strlen("닉네임을 입력해주세요:"));
                }
                else
                { // IO
                    readfd = fds[i].fd;
                    readLen = read(readfd, rBuff, sizeof(rBuff) - 1);
                    if (readLen == 0)
                    {
                        std::cerr << "A client is disconnected..." << std::endl;
                        close(readfd);
                        fds[i].fd = -1;
                    }
                    else if (readLen == -1 && errno != EWOULDBLOCK)
                    {
                        std::cerr << "Read error" << std::endl;
                        close(readfd);
                        fds[i].fd = -1;
                    }
                    else if (readLen > 0)
                    {
                        rBuff[readLen] = '\0';

                        if (nickNames[i].empty()) {
                            rBuff[readLen - 1] = '\0';
                            nickNames[i] = std::string(rBuff);
                            std::cout << nickNames[i] << "님이 입장하셨습니다." << std::endl;
                        } else {
                            std::cout << nickNames[i] << " : " << rBuff << std::endl;
                        
                            // send message to all clients
                            for (int j = 1; j < MAX_EVENTS + 1; j++) {
                                std::string newString = nickNames[i] + " : " + rBuff;
                                write(fds[j].fd, newString.c_str(), newString.size());
                            }
                        }
                    }
                }
            }
        }
	}
}

void Server::destroy() {
	close(Server::listenSd);
}

void errProc(const char* str)
{
    std::cerr << str << ": " << strerror(errno) << std::endl;
    exit(1);
}