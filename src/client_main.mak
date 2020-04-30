client_main: client_main.o wrap_string.o process_binary.o
	g++ client_main.o wrap_string.o process_binary.o -o client_main.exe -lws2_32

client_main.o: client_main.cpp
	g++ client_main.cpp -c -lws2_32

wrap_string.o: wrap_string.cpp
	g++ wrap_string.cpp -c
	
process_binary.o: process_binary.cpp
	g++ process_binary.cpp -c -lws2_32