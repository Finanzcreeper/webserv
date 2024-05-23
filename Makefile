SRCS = testmain.cpp Server.cpp httpParser.cpp MethodExecutor.cpp

OBJS = ${SRCS:.cpp=.o}

NAME = server

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