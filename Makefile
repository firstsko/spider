server: server.o sock.o event_driver.o     
	g++ -Wall -I../common -o server server.o sock.o event_driver.o -g -lpthread 
server.o:server.cpp
	g++ -Wall -o server.o server.cpp -g -c
sock.o:sock.cpp
	g++ -Wall -o sock.o sock.cpp -g -c
event_driver.o:event_driver.cpp
	g++ -Wall -o event_driver.o event_driver.cpp -g -c
clean:
	rm -rf server *.o
