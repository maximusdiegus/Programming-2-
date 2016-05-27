CC = g++
CFLAGS = -Wall -g
MODULOS = Fighter.o Fleet.o Ship.o Util.o imperialCommander.o

imperialCommander : $(MODULOS)
	$(CC) $(CFLAGS) $(MODULOS) -o hola

Fighter.o : Fighter.cc Fighter.h
	$(CC) $(CFLAGS) -c Fighter.cc

Fleet.o : Fleet.cc Fleet.h
	$(CC) $(CFLAGS) -c Fleet.cc

Ship.o : Ship.cc Ship.h
	$(CC) $(CFLAGS) -c Ship.cc

Util.o : Util.cc Util.h
	$(CC) $(CFLAGS) -c Util.cc

imperialCommander.o : imperialCommander.cc Fighter.h Fleet.h Ship.h Util.h
	$(CC) $(CFLAGS) -c imperialCommander.cc
clean:
	rm -rf $(MODULOS)
