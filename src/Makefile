SRC = *.cpp
CC = g++
CFLAG = -g -ggdb -Wall 
INCLUDE = -I ../proto
LINK = -L ../lib
LINKER = -lmsg

EXE = server
BIN = ../bin

all:
	$(CC) $(CFLAG) $(LINK) $(LINKER) $(INCLUDE) $(SRC) -o $(EXE)
	cp $(EXE) $(BIN)
clean:
	rm -rf *.o 
	rm -rf $(EXE)
