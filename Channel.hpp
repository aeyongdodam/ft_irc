#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include "Client.hpp"
#include "Server.hpp"

#define UNCONNECTED 0
#define CONNECTED 1
#define INVITED 2
#define BANNED 3

class Channel 
{
	private:
		Client		admin;
		std::string	name;
		std::string	*topic;
		std::string	*key;
		bool		inviteOnly;
		int			clientStatus[MAX_EVENTS + 1];
		int			maxCapacity;

		Channel();

	public:
		Channel(std::string name);
		Channel(const Channel& copy);
		Channel& operator=(const Channel& copy);
		~Channel();
};

#endif
