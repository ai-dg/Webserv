SRC = src/core/main.cpp \
	  src/core/Conf.cpp \
      src/core/Server.cpp \
	  src/core/HttpRequest.cpp \
	  src/core/HttpResponse.cpp \
      src/core/handler.cpp \
	  src/core/Epoll.cpp \
      src/cgi/cgi_handler.cpp \
      src/utils/logger.cpp \
	  src/utils/date.cpp \
	  src/utils/format.cpp \
	  src/utils/debugTools.cpp \
	  src/utils/files.cpp \
	  src/utils/parser.cpp \
	  src/utils/stringUtils.cpp \


OBJ = $(SRC:.cpp=.o)

NAME = webserv

CXX = c++
#CXXFLAGS = -Wall -Wextra -Werror -Wshadow -Wno-shadow -std=c++98
CXXFLAGS = -Wshadow -Wno-shadow -std=c++98 -lz

all: $(NAME)

$(NAME): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $(NAME) $(OBJ)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ)

fclean: clean
	rm -rf $(NAME)

re: fclean all

.PHONY: all clean fclean re
