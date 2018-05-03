# change application name here (executable output name)
TARGET=arcmusicplayer

# compiler
CC=g++
# debug
DEBUG=-g -O0
# warnings
WARN=-Wall

CCFLAGS=$(WARN) --std=c++11

GTKLIB=`pkg-config --cflags --libs gtkmm-3.0`
SDLLIB=`sdl2-config --cflags --libs` -l SDL2_mixer
NOTIFYLIB=`pkg-config --cflags --libs libnotify`
LIBS=$(SDLLIB) $(GTKLIB) $(NOTIFYLIB)

# linker
LDFLAGS=-export-dynamic

debug: CCFLAGS += $(DEBUG)
debug: amp

amp:
	$(CC) $(CCFLAGS) arcmusicplayer.cpp $(LIBS) -o $(TARGET) $(LDFLAGS)

clean:
	rm -f $(TARGET)
