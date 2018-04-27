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

# linker
LDFLAGS=$(SDLLIB) $(GTKLIB) -export-dynamic

debug: CCFLAGS += $(DEBUG)
debug: amp

amp:
	$(CC) $(CCFLAGS) arcmusicplayer.cpp $(SDLLIB) $(GTKLIB) -o $(TARGET) $(LDFLAGS)

clean:
	rm -f $(TARGET)
