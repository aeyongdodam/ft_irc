NAME	=	ircserv

SRCS	=	main.cpp Server.cpp Client.cpp Channel.cpp
SRCS	+=	command/pass.cpp command/nick.cpp
OBJS	=	$(SRCS:%.cpp=%.o)
HEADER	=	Server.hpp Client.hpp Channel.hpp 
HEADER	+=	command/command.hpp

CXX		=	c++
CXXFLAGS	=	-Wall -Werror -Wextra -std=c++98

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
