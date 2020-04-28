client_main: client_main.o wrap_string.o
	g++ client_main.o wrap_string.o -o client_main.exe -lws2_32

client_main.o: client_main.cpp
	g++ client_main.cpp -c -lws2_32

wrap_string.o: wrap_string.cpp
	g++ wrap_string.cpp -c