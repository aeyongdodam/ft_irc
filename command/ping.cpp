#include "command.hpp"

std::string PING(std::string str)
{
	std::string message;

	message = ":";
	message += str;
	message += " PONG ";
	message += str;
	return (message);
}
