server_main: server_main.o  process_binary.o server.o
	g++ server_main.o process_binary.o server.o -o serverd.exe -lws2_32
	
server.o: server.cpp
	g++ server.cpp -c -lws2_32
	
server_main.o: server_main.cpp
	g++ server_main.cpp -c
	
process_binary.o: process_binary.cpp
	g++ process_binary.cpp -c