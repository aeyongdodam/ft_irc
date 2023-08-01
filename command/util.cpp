#include "command.hpp"

std::pair<std::string, std::string> splitByFirstSpace(const std::string& input)
{
    std::size_t spacePos = input.find(' ');
    
    if (spacePos == std::string::npos) {
        return std::make_pair(input, "");
    } else {
        std::string firstPart = input.substr(0, spacePos);
        std::string secondPart = input.substr(spacePos + 1);
        return std::make_pair(firstPart, secondPart);
    }
}

std::list<std::string> split(std::string input, char delimiter)
{
    std::list<std::string> channels;
    std::stringstream ss(input);
    std::string temp;

    while (getline(ss, temp, delimiter))
        channels.push_back(temp);

    return channels;
}