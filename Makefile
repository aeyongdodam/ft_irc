NAME	=	ircserv

SRCS	=	main.cpp Server/Server.cpp Client/Client.cpp Channel/Channel.cpp
SRCS	+=	command/pass.cpp command/nick.cpp command/user.cpp command/privmsg.cpp \
			command/kick.cpp command/join.cpp command/part.cpp command/topic.cpp \
			command/mode.cpp command/quit.cpp command/util.cpp command/invite.cpp \
			command/ping.cpp
OBJS	=	$(SRCS:%.cpp=%.o)
HEADER	=	Server/Server.hpp Client/Client.hpp Channel/Channel.hpp 
HEADER	+=	command/command.hpp

CXX		=	c++
CXXFLAGS	=	-pedantic -Wall -Werror -Wextra -std=c++98

all		:	$(NAME)

$(NAME)	:	$(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(NAME)

%.o	:%.c
	$(CXX) $(CXXFLAGS) -c $< -o $@ -include $(HEADER)

clean	:
	rm -rf $(OBJS)

fclean	: clean
	rm -rf	$(NAME)

re		: fclean all

.PHONY: all clean fclean re
