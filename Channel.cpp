#include "Channel.hpp"

Channel::Channel() {}

Channel::Channel(std::string name) : name(name), topic(NULL), key(NULL), inviteOnly(false), maxCapacity(-1)
{
	for (int i = 0; i < MAX_EVENTS; i++)
		clientStatus[i] = 0;
	(void)topic;
	(void)key;;
	(void)inviteOnly;;
	(void)maxCapacity;;
}

Channel::Channel(const Channel& other) 
{
	(void) other;
}

Channel& Channel::operator=(const Channel& source) 
{
	(void)source;	
	return (*this);
}

Channel::~Channel() {}

