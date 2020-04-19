output: main.o server.o
	g++ main.o server.o -lws2_32 -o output


main.o: main.cpp server.h
	g++ main.cpp -c

server.o: server.cpp server.h
	g++ server.cpp -c

clean:
	del main.o server.o