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

#include <libnotify/notify.h>

#include "SDL.h"
#include "SDL_mixer.h"

class ArcMusicPlayer {
	Gtk::Window *aboutWindow;
	Gtk::Window *mainWindow;

	Gtk::ToggleButton *enableShuffle;
	Gtk::ToggleButton *enableFullPath;
	Gtk::ComboBox *repeatMode;
	Gtk::ToggleButton *saveState;
	Gtk::Scale *volumeSlider;

	int currentSongIndex = 0;
	std::vector<std::string> playlist;
	Gtk::ComboBoxText *playlistModel;

	unsigned int startTicks;
	int lastPos;
	bool isPlaying = false;

	/**
	 * Show about window
	 */
	void about();

	/**
	 * Rewind or fast forward by the given time difference
	 * @param delta Time to rewind or seek in seconds
	 */
	void movePos(int delta);

	/**
	 * React to volume change from UI
	 */
	void volumeChanged();

	/**
	 * Rewind 10 seconds
	 */
	void rw10();

	/**
	 * Rewind 30 seconds
	 */
	void rw30();

	/**
	 * Fast forward 10 seconds
	 */
	void ff10();

	/**
	 * Fast forward 30 seconds
	 */
	void ff30();

	/**
	 * Toggle playback
	 */
	void playpause();

	/**
	 * Play the currently selected song
	 */
	void playSong();

	/**
	 * Indicates whether the playlist is being modified
	 * internally; prevents userChoseSong from responding
	 * to internal actions
	 */
	bool isAlteringPlaylist = 0;

	/**
	 * React to user choosing a different song in the dropdown
	 */
	void userChoseSong();

	/**
	 * Go back to the previous song
	 */
	void prevSong();

	/**
	 * Clear the user's playlist
	 */
	void clearPlaylist();

	/**
	 * Write the current playlist to disk
	 */
	void savePlaylist();

	/**
	 * Load a playlist from disk
	 */
	void loadPlaylist();

	/**
	 * Add songs to the playlist
	 */
	void addSongs();

	/**
	 * Remove songs from the playlist
	 */
	void removeSongs();

	/**
	 * Update the dropdown menu to reflect the contents
	 * of the playlist
	 */
	void updatePlaylist();
public:
	ArcMusicPlayer();

	Glib::Dispatcher dispatcher;
	Mix_Music *music;

	/**
	 * Go to the next song
	 */
	void nextSong();

	int run(int, char**);
};
