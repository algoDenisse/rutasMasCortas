# change application name here (executable output name)
TARGET=Algoritmos
PENDING=pending_program
FLOYD=floyd

# compiler
CC=gcc
# debug
DEBUG=-g
# optimisation
OPT=-O0
# warnings
WARN=-Wall

PTHREAD=-pthread

CCFLAGS=$(DEBUG) $(OPT) $(WARN) $(PTHREAD) -pipe

GTKLIB=`pkg-config --cflags --libs gtk+-3.0`

# linker
LD=gcc
LDFLAGS=$(PTHREAD) $(GTKLIB) -export-dynamic

OBJS= main.o
POBJ= pending_program.o
FOBJ= floyd.o


all: $(OBJS) $(POBJ) $(FOBJ)
	$(LD) -o $(TARGET) $(OBJS) $(LDFLAGS)
	$(LD) -o $(PENDING) $(POBJ) $(LDFLAGS)
	$(LD) -o $(FLOYD) $(FOBJ) $(LDFLAGS)



main.o: src/main.c

	$(CC) -c $(CCFLAGS) src/main.c $(GTKLIB) -o main.o -w

pending_program.o: src/pending_program.c

	$(CC) -c $(CCFLAGS) src/pending_program.c $(GTKLIB) -o pending_program.o -w

floyd.o: src/floyd.c

	$(CC) -c $(CCFLAGS) src/floyd.c $(GTKLIB) -o floyd.o -w



clean:
	rm -f *.o $(TARGET)
