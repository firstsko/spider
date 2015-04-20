server: server.o event_driver.o sock.o timer.o log.o ini.o channel.o
	g++ -Wall -o server server.o sock.o timer.o event_driver.o log.o ini.o channel.o -g -lrt
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
clean:
	rm -rf server *.o
