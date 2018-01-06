# change application name here (executable output name)
TARGET=arcmusicplayer

# compiler
CC=g++
# debug
DEBUG=-g -O0
# warnings
WARN=-Wall

PTHREAD=-pthread

CCFLAGS=$(WARN) $(PTHREAD) -pipe --std=c++11

GTKLIB=`pkg-config --cflags --libs gtkmm-3.0`

# linker
LD=g++
LDFLAGS=$(PTHREAD) $(GTKLIB) -export-dynamic

OBJS=arcmusicplayer.o

all: $(OBJS)
	$(LD) -o $(TARGET) $(OBJS) $(LDFLAGS)

arcmusicplayer.o:
	$(CC) -c $(CCFLAGS) arcmusicplayer.cpp $(GTKLIB) -o arcmusicplayer.o

clean:
	rm -f *.o $(TARGET)
