CC = c++
FLAG = -Wall -Werror -Wextra -std=c++98 -g

INC_DIR = includes/
INC_FILE = webserv.hpp
INC	= $(addprefix $(INC_DIR), $(INC_FILE))

MAIN = main.cpp

CSERV_DIR = Server/
CSERV = Server.cpp

REQ_DIR = Request/
REQ = Request.cpp

RESP_DIR = Response/
RESP = Response.cpp AExcept.cpp ErrorExcept.cpp RedirExcept.cpp DirExcept.cpp

EPOL_DIR = Epoll/
EPOL = EpollCreation.cpp

CLI_DIR = Client/
CLI = Client.cpp

MIME_DIR = Mime/
MIME = Mime.cpp

CONF_DIR = Config/
CONF = AConfig.cpp ALocation.cpp MyConf.cpp MyLoc.cpp

HAND_DIR = Handler/
HAND = Handler.cpp

FILE_DIR = FileFd/
FILE = FileFd.cpp

SIGNAL_dir = signal/
SIGNAL_CPP = Signal.cpp

CLASS_DIR = class/
CLASS_SUB_DIR = $(addprefix $(CLASS_DIR), $(CSERV_DIR)) \
				$(addprefix $(CLASS_DIR), $(REQ_DIR)) \
				$(addprefix $(CLASS_DIR), $(RESP_DIR)) \
				$(addprefix $(CLASS_DIR), $(EPOL_DIR)) \
				$(addprefix $(CLASS_DIR), $(CLI_DIR)) \
				$(addprefix $(CLASS_DIR), $(MIME_DIR)) \
				$(addprefix $(CLASS_DIR), $(CONF_DIR)) \
				$(addprefix $(CLASS_DIR), $(HAND_DIR)) \
				$(addprefix $(CLASS_DIR), $(FILE_DIR)) \
				$(addprefix $(CLASS_DIR), $(SIGNAL_dir))


CLASS = $(addprefix $(CSERV_DIR), $(CSERV)) \
		$(addprefix $(REQ_DIR), $(REQ)) \
		$(addprefix $(RESP_DIR), $(RESP)) \
		$(addprefix $(EPOL_DIR), $(EPOL)) \
		$(addprefix $(CLI_DIR), $(CLI)) \
		$(addprefix $(MIME_DIR), $(MIME)) \
		$(addprefix $(CONF_DIR), $(CONF)) \
		$(addprefix $(HAND_DIR), $(HAND)) \
		$(addprefix $(FILE_DIR), $(FILE)) \
		$(addprefix $(SIGNAL_dir), $(SIGNAL_CPP))

CGI_DIR = cgi/
CGI = cgi.cpp

UTILS_DIR = utils/
UTILS = stringUtils.cpp fileUtils.cpp messageUtils.cpp displayUtils.cpp \
		units.cpp conversion.cpp searchingUtils.cpp dirListing.cpp handleUtils.cpp

SERV_DIR = server/
SERV = launchServer.cpp parseConfig.cpp eventHandling.cpp initServer.cpp

SRC_DIR = srcs/
SRC = $(addprefix $(SRC_DIR), $(MAIN)) \
		$(addprefix $(SRC_DIR), $(addprefix $(CGI_DIR), $(CGI))) \
		$(addprefix $(SRC_DIR), $(addprefix $(UTILS_DIR), $(UTILS))) \
		$(addprefix $(SRC_DIR), $(addprefix $(CLASS_DIR), $(CLASS))) \
		$(addprefix $(SRC_DIR), $(addprefix $(SERV_DIR), $(SERV)))

OBJ_DIR = obj/
OBJ_SUB_DIR = $(addprefix $(OBJ_DIR), $(CLASS_SUB_DIR)) \
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

$(NAME) : $(OBJ_SUB_DIR) $(OBJ)
	$(CC) $(FLAG) $(OBJ) -o $(NAME)

$(OBJ_SUB_DIR) :
	mkdir -p $(OBJ_SUB_DIR)

$(OBJ) : $(OBJ_DIR)%.o:$(SRC_DIR)%.cpp $(INC)
	$(CC) $(FLAG) -c $< -o $@ -I $(INC_DIR)

clean :
	rm -rf $(OBJ)
	rm -rf $(OBJ_DIR)

fclean : clean
	rm -f $(NAME)

re : fclean all