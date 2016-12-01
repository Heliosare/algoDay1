NAME    =       server
CC      =       g++
CFLAGS  =       -std=c++11 -O2
SRC     =       main.cc	\
                parser.cc \
                graph.cc
HEADERS = -I./headers/
OBJ = $(SRC:.cc=.o)
RM =       rm -rf

all:    $(OBJ)
	$(CC) $(CFLAGS) -o $(NAME) $(SRC) $(HEADERS)

clean:
	$(RM) *.o

fclean:  clean
	$(RM) $(NAME)
