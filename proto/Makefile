SRC = *.cc
CC = g++
CFLAG = -g -m64 -fPIC -ggdb -Wall -c -Wno-deprecated

EXE = libmsg.a
BIN = ../bin
all:
	protoc -I=. --cpp_out=. *.proto
	$(CC) $(SRC) $(CFLAG)
	ar -crv $(EXE) *.o
	cp $(EXE) $(BIN)
clean:
	rm -rf *.o 
	rm -rf $(EXE)

