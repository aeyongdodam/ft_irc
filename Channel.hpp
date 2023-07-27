#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include "Client.hpp"
#include "Server.hpp"
#include <ctime>

#define UNCONNECTED 0
#define CONNECTED 1
#define INVITED 2
#define BANNED 3

class Channel 
{
	private:
		int					adminId;
		const std::string	name;
		std::string			*topic;
		std::string			*key;
		bool				inviteOnly;
		bool				topicSetting; // 0이면 방장만, 1이면 모두 가능
		int					clientStatus[MAX_EVENTS + 1];
		int					capacity;
		int					maxCapacity;
		int					lastTopicSetId;
		long				lastTopicSetTime;

		Channel();

	public:
		Channel(int adminId, std::string& name);
		Channel(const Channel& copy);
		Channel& operator=(const Channel& copy);
		~Channel();

		int	joinChannel(int clientId);
		int joinChannel(int clientId, std::string& key);
		int inviteClient(int adminId, int targetId);
		int partClient(int clientId);
        int kickClient(int adminId, int targetId);
		int banClient(int adminId, int targetId);

		int changeInviteOnly(int adminId, bool inviteOnly);
		int changeTopic(int adminId, std::string& topic);
		int changeKey(int adminId, std::string* key);
		int changeAdmin(int oldAdminId, int newAdminId);

		int* getClientStatus();
		const std::string getName();
		std::string* getTopic();
		std::string* getKey();
		int getAdminId();
		bool gettopicSetting();
		std::string getClientList();
		
};

#endif
