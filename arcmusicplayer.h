//Copyright (C) 2018 Arc676/Alessandro Vinciguerra

//This program is free software: you can redistribute it and/or modify
//it under the terms of the GNU General Public License as published by
//the Free Software Foundation (version 3)

//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU General Public License for more details.

//You should have received a copy of the GNU General Public License
//along with this program.  If not, see <http://www.gnu.org/licenses/>.
//See README and LICENSE for more details

#include <gtkmm.h>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <fstream>

#include "SDL.h"
#include "SDL_mixer.h"

class ArcMusicPlayer {
	Gtk::Window *aboutWindow;
	Gtk::Window *mainWindow;

	std::vector<std::string> playlist;

	unsigned int startTicks;
	int lastPos;

	void about();

	void movePos(int);

	void rw10();
	void rw30();

	void ff10();
	void ff30();

	void playpause();

	void clearPlaylist();
	void savePlaylist();
	void loadPlaylist();

	void addSongs();
	void removeSongs();
	void updatePlaylist();
public:
	ArcMusicPlayer();

	Mix_Music *music;
	void nextSong();

	int run(int, char**);
};
