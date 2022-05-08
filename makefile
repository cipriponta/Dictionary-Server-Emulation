run: obj/client.o obj/server.o

obj/client.o: src/client.c obj/dependencies.o
	gcc src/client.c obj/dependencies.o -o runClient

obj/server.o: src/server.c obj/dependencies.o obj/dictionary.o
	gcc src/server.c obj/dependencies.o obj/dictionary.o -o runServer

obj/dictionary.o: src/dictionary.c src/dictionary.h
	gcc -c src/dictionary.c -o obj/dictionary.o

obj/dependencies.o: src/dependencies.c src/dependencies.h
	gcc -c src/dependencies.c -o obj/dependencies.o

clean:
	rm obj/*.o
	rm runClient
	rm runServer