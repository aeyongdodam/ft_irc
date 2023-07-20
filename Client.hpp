#ifndef CLIENT_HPP
#define CLIENT_HPP

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
#include <vector>

class Client
{
    private:
        std::string nickName;
        std::string loginName;
        std::string realName;
    public:
        void clientInfoinit(std::string input);
        std::string getNickName() const;
        std::string getLoginName() const;
        std::string getRealName() const;

};

#endif