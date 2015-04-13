login_server: login_server.o sock.o event_driver.o     
	g++ -Wall -I../common -o login_server login_server.o sock.o event_driver.o -g -lpthread 
login_server.o:login_server.cpp
	g++ -Wall -o login_server.o login_server.cpp -g -c
sock.o:sock.cpp
	g++ -Wall -o sock.o sock.cpp -g -c
event_driver.o:event_driver.cpp
	g++ -Wall -o event_driver.o event_driver.cpp -g -c
clean:
	rm -rf login_server *.o
