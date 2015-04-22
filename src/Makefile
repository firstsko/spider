server: server.o event_driver.o sock.o timer.o log.o ini.o channel.o event_handler.o message.o
	g++ -Wall -o server server.o sock.o timer.o event_driver.o log.o ini.o channel.o event_handler.o message.o -g -lrt
server.o:server.cpp
	g++ -Wall -o server.o server.cpp -g -c
sock.o:sock.cpp
	g++ -Wall -o sock.o sock.cpp -g -c
timer.o:timer.cpp
	g++ -Wall -o timer.o timer.cpp -g -c
event_driver.o:event_driver.cpp
	g++ -Wall -o event_driver.o event_driver.cpp -g -c
log.o:log.cpp
	g++ -Wall -o log.o log.cpp -g -c
ini.o:ini.cpp
	g++ -Wall -o ini.o ini.cpp -g -c
channel.o:channel.cpp
	g++ -Wall -o channel.o channel.cpp -g -c
event_handler.o:event_handler.cpp
	g++ -Wall -o event_handler.o event_handler.cpp -g -c
message.o:message.cpp
	g++ -Wall -o message.o message.cpp -g -c
all:
	mv server ../bin
clean:
	rm -rf server *.o
