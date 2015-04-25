SRC = *.cc
CC = g++
CFLAG = -g -m64 -fPIC -ggdb -Wall -c -Wno-deprecated

EXE = libmsg.a
LIB = ../lib
all:
	protoc -I=. --cpp_out=. *.proto
	$(CC) $(SRC) $(CFLAG)
	ar -crv $(EXE) *.o
	cp $(EXE) $(LIB)
clean:
	rm -rf *.o 
	rm -rf $(EXE)

