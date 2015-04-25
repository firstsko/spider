SRC = *.cpp
CC = g++
CFAG = -g -ggdb -Wall -Wno-deprecated
INCLUDE = -I ../proto

EXE = server
BIN = ../bin

all:
	$(CC) $(CFLAG)  $(INCLUDE) $(SRC) -o $(EXE)
	cp $(EXE) $(BIN)
clean:
	rm -rf *.o 
	rm -rf $(EXE)
