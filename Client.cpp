#include "Client.hpp"

void Client::clientInfoinit(const std::string input)
{
    const char* str = input.c_str();
    if (std::strncmp(str, "NICK ", 5) == 0 || std::strncmp(str, "USER ", 5) == 0)
    {
        std::string tmp = std::strchr(str, ' ');
        if (tmp.c_str() != NULL)
        {
            if (std::strncmp(str, "NICK ", 5) == 0)
                nickName = tmp;
            else if (std::strncmp(str, "USER ", 5) == 0)
            {
                std::vector<std::string> result;
                const char* str = tmp.c_str();
                const char* token = std::strtok(const_cast<char*>(str), " ");
                while (token != NULL) 
                {
                    result.push_back(std::string(token));
                    token = std::strtok(NULL, " ");
                }
                if (result.size() >= 4)
                {
                    loginName = result[0];
                    realName = result[3];
                }
            }

        }
    }   
}

std::string Client::getNickName() const
{
    return nickName;
}

std::string Client::getLoginName() const
{
    return loginName;
}

std::string Client::getRealName() const
{
    return realName;
}