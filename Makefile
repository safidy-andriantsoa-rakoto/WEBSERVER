CC = c++
FLAG = -Wall -Werror -Wextra -std=c++98 -g

INC_DIR = includes/
INC_FILE = webserv.hpp
INC	= $(addprefix $(INC_DIR), $(INC_FILE))

MAIN = main.cpp

SERV_DIR = Server/
SERV = Server.cpp

REQ_DIR = Request/
REQ = Request.cpp

RESP_DIR = Response/
RESP = Response.cpp AExcept.cpp ErrorExcept.cpp RedirExcept.cpp LDirExcept.cpp

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

SIGNAL_DIR = Signal/
SIGNAL_CPP = Signal.cpp

CGIPAR_DIR = CgiParser/
CGIPAR = CgiParser.cpp

CGIFD_DIR = CgiFd/
CGIFD = CgiFd.cpp

CLASS_DIR = class/
CLASS_SUB_DIR = $(addprefix $(CLASS_DIR), $(SERV_DIR)) \
				$(addprefix $(CLASS_DIR), $(REQ_DIR)) \
				$(addprefix $(CLASS_DIR), $(RESP_DIR)) \
				$(addprefix $(CLASS_DIR), $(EPOL_DIR)) \
				$(addprefix $(CLASS_DIR), $(CLI_DIR)) \
				$(addprefix $(CLASS_DIR), $(MIME_DIR)) \
				$(addprefix $(CLASS_DIR), $(CONF_DIR)) \
				$(addprefix $(CLASS_DIR), $(HAND_DIR)) \
				$(addprefix $(CLASS_DIR), $(FILE_DIR)) \
				$(addprefix $(CLASS_DIR), $(SIGNAL_DIR)) \
				$(addprefix $(CLASS_DIR), $(CGIPAR_DIR)) \
				$(addprefix $(CLASS_DIR), $(CGIFD_DIR))


CLASS = $(addprefix $(SERV_DIR), $(SERV)) \
		$(addprefix $(REQ_DIR), $(REQ)) \
		$(addprefix $(RESP_DIR), $(RESP)) \
		$(addprefix $(EPOL_DIR), $(EPOL)) \
		$(addprefix $(CLI_DIR), $(CLI)) \
		$(addprefix $(MIME_DIR), $(MIME)) \
		$(addprefix $(CONF_DIR), $(CONF)) \
		$(addprefix $(HAND_DIR), $(HAND)) \
		$(addprefix $(FILE_DIR), $(FILE)) \
		$(addprefix $(SIGNAL_DIR), $(SIGNAL_CPP)) \
		$(addprefix $(CGIPAR_DIR), $(CGIPAR)) \
		$(addprefix $(CGIFD_DIR), $(CGIFD))

UTILS_DIR = utils/
UTILS = stringUtils.cpp fileUtils.cpp displayUtils.cpp \
		units.cpp conversion.cpp searchingUtils.cpp dirListing.cpp handleUtils.cpp

PRO_DIR = program/
PRO = launchServer.cpp parseConfig.cpp eventHandling.cpp initServer.cpp sendingResponse.cpp

SRC_DIR = srcs/
SRC = $(addprefix $(SRC_DIR), $(MAIN)) \
		$(addprefix $(SRC_DIR), $(addprefix $(UTILS_DIR), $(UTILS))) \
		$(addprefix $(SRC_DIR), $(addprefix $(CLASS_DIR), $(CLASS))) \
		$(addprefix $(SRC_DIR), $(addprefix $(PRO_DIR), $(PRO)))

OBJ_DIR = obj/
OBJ_SUB_DIR = $(addprefix $(OBJ_DIR), $(CLASS_SUB_DIR)) \
				$(addprefix $(OBJ_DIR), $(UTILS_DIR)) \
				$(addprefix $(OBJ_DIR), $(PRO_DIR))

OBJ_MAIN = $(addprefix $(OBJ_DIR), $(MAIN:.cpp=.o))
OBJ_CLASS = $(addprefix $(OBJ_DIR), $(addprefix $(CLASS_DIR), $(CLASS:.cpp=.o)))
OBJ_UTILS = $(addprefix $(OBJ_DIR), $(addprefix $(UTILS_DIR), $(UTILS:.cpp=.o)))
OBJ_PRO = $(addprefix $(OBJ_DIR), $(addprefix $(PRO_DIR), $(PRO:.cpp=.o)))
OBJ = $(OBJ_MAIN) $(OBJ_CLASS) $(OBJ_UTILS) $(OBJ_PRO)

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