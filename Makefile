FLAGS = -Wall -Werror -Wextra -std=c++98

NAME = webserv

SRCS = server.cpp

CC = c++

OBJS = $(SRCS:.cpp=.o)

%.o:%.cpp
	$(CC) $(FLAGS) -c $< -o $@

$(NAME) : $(OBJS)
	$(CC) $(FLAGS) $(OBJS) -o $(NAME)

all: $(NAME)

clean:
	@rm -rf $(OBJS)

fclean: clean
	@rm -f $(NAME)

re:	fclean all