SRCS =	server/MethodExecutor.cpp server/statusCodes.cpp server/Server.cpp \
		server/headerGeneration.cpp server/bodyGeneration.cpp \
		parsers/ConfigParse.cpp parsers/ConfigLocationParse.cpp parsers/httpParser.cpp\
		interpreters/httpInterpreter.cpp \
		tests/Tests.cpp

MAIN = main.cpp
TESTMAIN =	testmain.cpp

MAKEFLAGS += --no-print-directory

OBJS = $(SRCS:.cpp=.o)
MAINOBJ = $(MAIN:.cpp=.o)
TESTMAINOBJ =$(TESTMAIN:.cpp=.o)
NAME = webserv
TESTNAME = webtest

CXX = c++
CXXFLAGS = -Wall -Werror -Wextra -std=c++98 -g3 #-fsanitize=thread

.SILENT:

all: $(NAME)

%.o: %.cpp | objectEcho
	$(CXX) $(CXXFLAGS) -c $< -o $@

objectEcho:
	echo -e "\033[1;32mCreating Objects\033[0m"

$(NAME): $(MAINOBJ) $(OBJS)
	echo -e "\033[1;32mCompiling files\033[0m"
	$(CXX) $(CXXFLAGS) $^ -o $(NAME)

 test: $(TESTMAINOBJ) $(OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $(TESTNAME)

clean:
	rm -f $(OBJS) $(MAINOBJ) $(TESTMAINOBJ) *.o
	echo -e "\033[1;31mCleaning\033[0m "

fclean: clean
	rm -f $(NAME) $(TESTNAME)
	echo -e "\033[1;31mEverything\033[0m"

re: fclean all

.PHONY: all clean fclean re test objectEcho