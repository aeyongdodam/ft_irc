#include "command.hpp"



int checkPassword(std::string pass, std::string password)
{
	int passflag = 0;
	if (pass.c_str() != NULL)
	{
		if (std::strncmp(password.c_str(), pass.c_str(), password.size()) == 0) // 고쳐야함
		{
			std::cout << "The password is correct" << std::endl;
			passflag = 1;
			return passflag;
		}
		else
		{
			std::cout << "The password is not correct" << std::endl;
			return passflag;
		}
	}
	std::cout << "The password is not correct" << std::endl;
	return passflag;
}