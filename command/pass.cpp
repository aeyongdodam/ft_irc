#include "command.hpp"

int checkPassword(std::string pass, int clientId)
{
	Server &server = Server::getInstance();
	Client *clients = server.getClients();

	if (clients[clientId].getPassFlag())
		return ERR_ALREADYREGISTERED;

	if (pass.c_str() != NULL)
	{
		if (std::strncmp(server.getGenernalPass().c_str(), pass.c_str(), server.getGenernalPass().size() + 1) == 0)
		{
			clients[clientId].setPassFlag(true);
			clients[clientId].setAdminFlag(false);
			return RPL_WELCOME;
		}
		else if (std::strncmp(server.getOperatorPass().c_str(), pass.c_str(), server.getOperatorPass().size() + 1) == 0)
		{
			clients[clientId].setPassFlag(true);
			(server.getClients()[clientId]).setAdminFlag(true);
			return RPL_WELCOME;
		}
		else
			return ERR_PASSWDMISMATCH;
	}
	else
		return ERR_PASSWDMISMATCH;
}

std::string makePassResponse(int responseCode)
{
	if (responseCode == RPL_WELCOME)
		return "";

	std::string resMsg;
	resMsg += std::to_string(responseCode);
	resMsg += " :";

	switch (responseCode)
	{
	case ERR_ALREADYREGISTERED:
		resMsg += "You may not reregister";
		break;
	case ERR_BANNEDFROMCHAN:
		resMsg += "Password incorrect";
	}
	return resMsg;
}

void PASS(std::string pass, int clientId)
{
	Server &server = Server::getInstance();
	Client* clients = server.getClients();

	int responseCode = checkPassword(pass, clientId);

	std::string resMsg = makePassResponse(responseCode);

	if (!resMsg.empty())
		server.sendMessage(clientId, resMsg);
	if (clients[clientId].getPassFlag() == false)
		server.disconnectClient(clientId, server.getFds()[clientId].fd);
}
