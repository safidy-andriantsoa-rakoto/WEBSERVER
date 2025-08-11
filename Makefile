CC = c++
FLAG = -Wall -Werror -Wextra -std=c++98 -g

INC_DIR = includes/
INC_FILE = webserv.hpp
INC	= $(addprefix $(INC_DIR), $(INC_FILE))

MAIN = main.cpp

CLASS_DIR = class/
CLASS = Server.cpp Request.cpp Response.cpp ErrorExcept.cpp EpollCreation.cpp Client.cpp \
		tempClass/AConfig.cpp tempClass/ALocation.cpp tempClass/MyConf.cpp tempClass/MyLoc.cpp

CGI_DIR = cgi/
CGI = cgi.cpp

UTILS_DIR = utils/
UTILS = stringUtils.cpp fileUtils.cpp messageUtils.cpp displayUtils.cpp

SERV_DIR = server/
SERV = launchServer.cpp parseConfig.cpp serverHandler.cpp

SRC_DIR = srcs/
SRC = $(addprefix $(SRC_DIR), $(MAIN)) \
		$(addprefix $(SRC_DIR), $(addprefix $(CGI_DIR), $(CGI))) \
		$(addprefix $(SRC_DIR), $(addprefix $(UTILS_DIR), $(UTILS))) \
		$(addprefix $(SRC_DIR), $(addprefix $(CLASS_DIR), $(CLASS))) \
		$(addprefix $(SRC_DIR), $(addprefix $(SERV_DIR), $(SERV)))

OBJ_DIR = obj/
OBJ_ALL_DIR = $(addprefix $(OBJ_DIR), $(CLASS_DIR)) \
				$(addprefix $(OBJ_DIR), $(CGI_DIR)) \
				$(addprefix $(OBJ_DIR), $(UTILS_DIR)) \
				$(addprefix $(OBJ_DIR), $(SERV_DIR))

OBJ_MAIN = $(addprefix $(OBJ_DIR), $(MAIN:.cpp=.o))
OBJ_CLASS = $(addprefix $(OBJ_DIR), $(addprefix $(CLASS_DIR), $(CLASS:.cpp=.o)))
OBJ_CGI = $(addprefix $(OBJ_DIR), $(addprefix $(CGI_DIR), $(CGI:.cpp=.o)))
OBJ_UTILS = $(addprefix $(OBJ_DIR), $(addprefix $(UTILS_DIR), $(UTILS:.cpp=.o)))
OBJ_SERV = $(addprefix $(OBJ_DIR), $(addprefix $(SERV_DIR), $(SERV:.cpp=.o)))
OBJ = $(OBJ_MAIN) $(OBJ_CLASS) $(OBJ_UTILS) $(OBJ_CGI) $(OBJ_SERV)

NAME = webserv

all : $(NAME)

$(NAME) : $(OBJ_ALL_DIR) $(OBJ)
	$(CC) $(FLAG) $(OBJ) -o $(NAME)

# Review this part !!!!
$(OBJ_ALL_DIR) :
	mkdir -p $(OBJ_ALL_DIR)
	mkdir obj/class/tempClass

$(OBJ) : $(OBJ_DIR)%.o:$(SRC_DIR)%.cpp $(INC)
	$(CC) $(FLAG) -c $< -o $@ -I $(INC_DIR)

clean :
	rm -rf $(OBJ)
	rm -rf $(OBJ_DIR)

fclean : clean
	rm -f $(NAME)

re : fclean all