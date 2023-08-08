#include "Server/Server.hpp"

int main(int argc, char** argv)
{
	if (argc != 3)
	{
		std::cout << "Usage: " << argv[0] << " [Port Number] [Password]" << std::endl;
		return -1;
	}

	Server& server = Server::getInstance();
	server.init(std::atoi(argv[1]), argv[2]);
	server.monitoring();
	server.destroy();
	
	return 0;
}
