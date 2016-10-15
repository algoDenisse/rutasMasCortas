# change application name here (executable output name)
TARGET=Algoritmos
PENDING=pending_program
FLOYD=floyd
KNAPSACK=knapsack
EQREPLACE=equipment_replace
SDEPORTIVAS=series_deportivas
BINARYTREE=binary_tree

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

GTKLIB=-lm `pkg-config --cflags --libs gtk+-3.0`

# linker
LD=gcc
LDFLAGS=$(PTHREAD) $(GTKLIB) -export-dynamic

OBJS= main.o
POBJ= pending_program.o
FOBJ= floyd.o
KNOBJ=knapsack.o
EROBJ=equipment_replace.o
SDOBJ=series_deportivas.o
BTOBJ=binary_tree.o

all: $(OBJS) $(POBJ) $(FOBJ) $(KNOBJ) $(EROBJ) $(SDOBJ) $(BTOBJ)
	$(LD) -o $(TARGET) $(OBJS) $(LDFLAGS)
	$(LD) -o $(PENDING) $(POBJ) $(LDFLAGS)
	$(LD) -o $(FLOYD) $(FOBJ) $(LDFLAGS)
	$(LD) -o $(KNAPSACK) $(KNOBJ) $(LDFLAGS)
	$(LD) -o $(EQREPLACE) $(EROBJ) $(LDFLAGS)
	$(LD) -o $(SDEPORTIVAS) $(SDOBJ) $(LDFLAGS)
	$(LD) -o $(BINARYTREE) $(BTOBJ) $(LDFLAGS)



main.o: src/main.c

	$(CC) -c $(CCFLAGS) src/main.c $(GTKLIB) -o main.o -w

pending_program.o: src/pending_program.c

	$(CC) -c $(CCFLAGS) src/pending_program.c $(GTKLIB) -o pending_program.o -w

floyd.o: src/floyd.c

	$(CC) -c $(CCFLAGS) src/floyd.c $(GTKLIB) -o floyd.o -w

knapsack.o: src/knapsack.c

	$(CC) -c $(CCFLAGS) src/knapsack.c $(GTKLIB) -o knapsack.o -w

equipment_replace.o: src/equipment_replace.c

	$(CC) -c $(CCFLAGS) src/equipment_replace.c $(GTKLIB) -o equipment_replace.o -w

series_deportivas.o: src/series_deportivas.c

	$(CC) -c $(CCFLAGS) src/series_deportivas.c $(GTKLIB) -o series_deportivas.o -w

binary_tree.o: src/binary_tree.c

	$(CC) -c $(CCFLAGS) src/binary_tree.c $(GTKLIB) -o binary_tree.o -w



clean:
	rm -f *.o $(TARGET)
