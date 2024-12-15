# Core src files
SRC = src/01-core/Conf.cpp \
	  src/01-core/Cookies.cpp \
	  src/01-core/Epoll.cpp \
	  src/01-core/HttpRequest.cpp \
	  src/01-core/HttpResponse.cpp \
	  src/01-core/index.cpp \
	  src/01-core/Location.cpp \
	  src/01-core/main.cpp \
	  src/01-core/Pipe.cpp \
	  src/01-core/RarManager.cpp \
	  src/01-core/RequestResponseManager.cpp \
	  src/01-core/scriptUtils.cpp \
      src/01-core/Server.cpp \
	  src/01-core/SessionManager.cpp \
	  src/01-core/SignalHandler.cpp \
	  src/01-core/Sockets.cpp \
	  src/01-core/Status.cpp \

# Utils src files
SRC += src/02-utils/cleanup.cpp \
	  src/02-utils/date.cpp \
	  src/02-utils/debugTools.cpp \
	  src/02-utils/directories.cpp \
	  src/02-utils/files.cpp \
	  src/02-utils/format.cpp \
	  src/02-utils/ipTools.cpp \
	  src/02-utils/Log.cpp \
	  src/02-utils/signals.cpp \
	  src/02-utils/parser.cpp \
	  src/02-utils/stringUtils.cpp \

# CGI src files
SRC += src/03-cgi/cgi_handler.cpp

# Exceptions src files
SRC += src/04-exceptions/InvalidArgException.cpp


OBJ = $(SRC:.cpp=.o)

NAME = webserv

CXX = c++
CXXFLAGS = -Wall -Wextra -Werror -Wshadow -Wno-shadow -std=c++98 -g
# CXXFLAGS = -Wshadow -Wno-shadow -std=c++98 -g

all: $(NAME) clean_temp

$(NAME): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $(NAME) $(OBJ)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ)

clean_temp:
	rm -rf ./sessions/* ./logs/*

fclean: clean
	rm -rf $(NAME)

re: fclean all

launch: clean_temp
	make
	./webserv

tests: clean_temp
	make
	./webserv config/tests.conf

test:
	./testers/ubuntu_tester http://127.1.1.1:8001

.PHONY: all clean fclean re clean_temp test launch
