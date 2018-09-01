# change application name here (executable output name)
TARGET=arcmusicplayer

# compiler
CC=g++
# debug
DEBUGFLAGS=-g -O0
# warnings
WARN=-Wall

CCFLAGS=$(WARN) --std=c++17

ifdef NOEXP
	CCFLAGS+=-D NOEXP=1
endif

GTKLIB=`pkg-config --cflags --libs gtkmm-3.0`
SDLLIB=`sdl2-config --cflags --libs` -l SDL2_mixer
NOTIFYLIB=`pkg-config --cflags --libs libnotify`
LIBS=$(SDLLIB) $(GTKLIB) $(NOTIFYLIB)

# linker
LDFLAGS=-l stdc++fs

ifdef DEBUG
	CCFLAGS += $(DEBUGFLAGS)
	LDFLAGS += -export-dynamic
endif

amp:
	$(CC) $(CCFLAGS) arcmusicplayer.cpp $(LIBS) -o $(TARGET) $(LDFLAGS)

clean:
	rm -f $(TARGET)
