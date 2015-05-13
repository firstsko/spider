SRC = *.cpp
CC = g++
CFLAG = -g -ggdb -Wall 
INCLUDE = -I ../proto
LINK = -L ../lib -L/usr/local/lib
LINKER =-lprotobuf -lmsg -lrt 
EXE = spider-server
BIN = ../bin

all:
	$(CC) $(INCLUDE) $(SRC) $(CFLAG) $(LINK) $(LINKER) -o $(EXE)
	../bin/admin.sh stop
	cp $(EXE) $(BIN)
	ctags -R *
clean:
	rm -rf *.o 
	rm -rf $(EXE)
