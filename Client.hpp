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
#include <list>

class Channel;

class Client
{
	private:
		std::string nickName;
		std::string loginName;
		std::string realName;
		bool passFlag;

		std::list<Channel*> channels;

	public:
		Client();
		Client(const Client& copy);
		Client& operator=(const Client& copy);
		~Client();
		

		std::string& getNickName();
		std::string& getLoginName();
		std::string& getRealName();
		std::list<Channel*>& getChannels();
		bool getPassFlag() const;
		void setNickName(std::string nickName_);
		void setLoginName(std::string loginName_);
		void setRealName(std::string setRealName_);
		void setPassFlag(bool passFlag_);

		void addChannel(Channel *new_channel);
};

#endif
