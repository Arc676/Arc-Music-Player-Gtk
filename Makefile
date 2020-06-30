# change application name here (executable output name)
TARGET=arcmusicplayer

CC=g++
DEBUGFLAGS=-g -O0
WARN=-Wall

CCFLAGS=$(WARN) --std=c++17

ifdef NOEXP
	CCFLAGS += -D NOEXP=1
endif

GTKLIB=`pkg-config --cflags --libs gtkmm-3.0`
SDLLIB=`sdl2-config --cflags --libs` -l SDL2_mixer
NOTIFYLIB=`pkg-config --cflags --libs libnotify`

LIBS=$(SDLLIB) $(GTKLIB) $(NOTIFYLIB)

LDFLAGS=-l stdc++fs

ifdef DEBUG
	CCFLAGS += $(DEBUGFLAGS)
	LDFLAGS += -export-dynamic
else
	CCFLAGS += -O2
endif

amp:
	$(CC) $(CCFLAGS) arcmusicplayer.cpp $(LIBS) -o $(TARGET) $(LDFLAGS)

clean:
	rm -f $(TARGET)
