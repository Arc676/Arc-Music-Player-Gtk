# change application name here (executable output name)
TARGET=arcmusicplayer

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

OBJS=arcmusicplayer.o

all: $(OBJS)
	$(LD) -o $(TARGET) $(OBJS) $(LDFLAGS)

arcmusicplayer.o:
	$(CC) -c $(CCFLAGS) arcmusicplayer.c $(GTKLIB) -o arcmusicplayer.o

clean:
	rm -f *.o $(TARGET)
