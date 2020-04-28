server_ui: server_ui.o wrap_string.o
	g++ server_ui.o wrap_string.o -o server_ui.exe

server_ui.o: server_ui.cpp
	g++ server_ui.cpp -c

wrap_string.o: wrap_string.cpp
	g++ wrap_string.cpp -c
