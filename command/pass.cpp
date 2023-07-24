#include "command.hpp"
#include "../Server.hpp"

int checkPassword(std::string pass, int clientId)
{
	Server& server = Server::getInstance();
	Client* clients = server.getClients();

	if (clients[clientId].getPassFlag())
	{
		return 462;
	}

 	int passflag = 0;
	if (pass.c_str() != NULL)
	{
		if (std::strncmp(server.getGenernalPass().c_str(), pass.c_str(), server.getGenernalPass().size() + 1) == 0)
		{
			std::cout << "The password is correct" << std::endl;
			clients[clientId].setAdminFlag(false);
		}
		else if (std::strncmp(server.getOperatorPass().c_str(), pass.c_str(), server.getOperatorPass().size() + 1) == 0)
		{
			std::cout << "The password is correct (Operator)" << std::endl;
			(server.getClients()[clientId]).setAdminFlag(true);
			passflag = 2;
		}
		else
			std::cout << "The password is not correct" << std::endl;
	}
	else
		std::cout << "The password is not correct" << std::endl;

	return passflag;
}