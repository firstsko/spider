SRC = *.cpp
CC = g++
CFAG = -g -ggdb -Wall -Wno-deprecated

EXE = server
BIN = ../bin

all:
	$(CC) $(CFLAG) $(SRC) -o $(EXE)
	cp $(EXE) $(BIN)
clean:
	rm -rf *.o 
	rm -rf $(EXE)
