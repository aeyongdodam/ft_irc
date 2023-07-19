#include "Server.hpp"

int main(int argc, char** argv)
{
    if (argc != 2)
    {
        std::cout << "Usage: " << argv[0] << " [Port Number]" << std::endl;
        return -1;
    }

    Server::init(std::atoi(argv[1]));
    Server::monitoring();
    Server::destroy();
    
    return 0;
}

