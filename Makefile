SRC = src/core/main.cpp \
      src/core/server.cpp \
      src/core/handler.cpp \
      src/cgi/cgi_handler.cpp \
      src/utils/logger.cpp

OBJ = $(SRC:.cpp=.o)

NAME = webserv

CXX = c++
CXXFLAGS = -Wall -Wextra -Werror -Wshadow -Wno-shadow -std=c++98

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
