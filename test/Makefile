SRC = *.cpp ../proto/*.cc
CC = g++
CFLAG = -g -ggdb -Wall -Wno-deprecated
INCLUDE = -I ../proto
LINK = -L ../lib -L /usr/local/lib
LIB = ../lib/libmsg.a
LINKER = -static -lprotobuf #-lmsg  
EXE = client

all:
	$(CC) $(CFLAG) $(LINK) $(LINKER) $(INCLUDE) $(SRC) -o $(EXE)
clean:
	rm -rf *.o 
	rm -rf $(EXE)
