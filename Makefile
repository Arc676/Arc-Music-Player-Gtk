# change application name here (executable output name)
TARGET=arcmusicplayer

# compiler
CC=g++
# debug
DEBUG=-g -O0
# warnings
WARN=-Wall

NOEXP=0
CCFLAGS=$(WARN) --std=c++17 -D NOEXP=$(NOEXP)

GTKLIB=`pkg-config --cflags --libs gtkmm-3.0`
SDLLIB=`sdl2-config --cflags --libs` -l SDL2_mixer
NOTIFYLIB=`pkg-config --cflags --libs libnotify`
LIBS=$(SDLLIB) $(GTKLIB) $(NOTIFYLIB)

# linker
LDFLAGS=-l stdc++fs

amp:
	$(CC) $(CCFLAGS) arcmusicplayer.cpp $(LIBS) -o $(TARGET) $(LDFLAGS)

clean:
	rm -f $(TARGET)

debug: CCFLAGS += $(DEBUG)
debug: LDFLAGS += -export-dynamic
debug: amp
