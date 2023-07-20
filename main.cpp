#include "Server.hpp"

int main(int argc, char** argv)
{
    if (argc != 3)
    {
        std::cout << "Usage: " << argv[0] << " [Port Number] [Password]" << std::endl;
        return -1;
    }
    
    Server::init(std::atoi(argv[1]));
    Server::monitoring(argv[2]);
    Server::destroy();
    
    return 0;
}
