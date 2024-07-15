SRCS =	server/MethodExecutor.cpp server/statusCodes.cpp server/Server.cpp \
		parsers/ConfigParse.cpp parsers/httpParser.cpp\
		parsers/ConfigLocationParse.cpp\
		testmain.cpp

OBJS = ${SRCS:.cpp=.o}

NAME = webserv

CXX = c++
CXXFLAGS = -Wall -Werror -Wextra -std=c++98 #-g #-fsanitize=thread

all: ${NAME}

%.o: %.c
	${CXX} ${CXXFLAGS} -c $< -o ${<:.c=.o}

${NAME}: ${OBJS}
	${CXX} ${CXXFLAGS} ${OBJS} -o ${NAME}

clean:
	rm -f ${OBJS}

fclean: clean
	rm -f ${NAME}

re: fclean all

.PHONY: all clean fclean re
