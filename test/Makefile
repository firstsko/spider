SRC = *.cpp
CC = g++
CFLAG =-g -Wall 
INCLUDE =-I../proto
LINK =-L../lib
LIB =../lib/libmsg.a
LINKER =-lprotobuf -lmsg  
EXE =client
all:
	$(CC) $(INCLUDE) $(SRC) $(CFLAG) $(LINK) $(LINKER) -o $(EXE)
clean:
	rm -rf *.o 
	rm -rf $(EXE)
