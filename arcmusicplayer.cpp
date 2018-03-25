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

#include "arcmusicplayer.h"

ArcMusicPlayer* amp;

void ArcMusicPlayer::about() {
	aboutWindow->show();
}

void ArcMusicPlayer::movePos(int delta) {
	Mix_RewindMusic();
	int pos = (SDL_GetTicks() - startTicks) / 1000;
	pos += delta;
	startTicks -= delta * 1000;
	Mix_SetMusicPosition((double)pos);
}

void ArcMusicPlayer::rw10() {
	movePos(-10);
}

void ArcMusicPlayer::rw30() {
	movePos(-30);
}

void ArcMusicPlayer::ff10() {
	movePos(10);
}

void ArcMusicPlayer::ff30() {
	movePos(30);
}

void musicStopped() {
	amp->nextSong();
}

void ArcMusicPlayer::playSong() {
	music = Mix_LoadMUS(playlist[currentSongIndex]);
	// update the slider once that's added
	playPause();
	// get song duration once you figure out how to do that
	updatePlaylist();
}

void ArcMusicPlayer::nextSong() {
	startTicks = SDL_GetTicks();
	if (playlist.size() == 0) {
		return;
	}
	if (enableShuffle->getActive()) {
		playlist.erase(playlist.begin() + currentSongIndex);
		if (playlist.size() == 0) {
			currentSongIndex = 0;
			updatePlaylist();
			return;
		}
		currentSongIndex = rand() * playlist.size();
	} else {
		int rep = repeatMode->get_active_row_number();
		if (playlist.size() > currentSongIndex + 1 || rep == 1) {
			if (rep != 1) {
				currentSongIndex++;
			}
		}
	}
	playSong();
}

void ArcMusicPlayer::addSongs() {
	Gtk::FileChooserDialog dialog("Select music files");
	dialog.set_transient_for(*mainWindow);

	dialog.add_button("Cancel", Gtk::RESPONSE_CANCEL);
	dialog.add_button("OK", Gtk::RESPONSE_OK);

	dialog.set_select_multiple();

	auto filter_text = Gtk::FileFilter::create();
	filter_text->set_name("Audio files");
	filter_text->add_mime_type("audio/mpeg");
	dialog.add_filter(filter_text);

	if (dialog.run() == Gtk::RESPONSE_OK) {
		std::vector<std::string> files = dialog.get_filenames();
		playlist.insert(playlist.end(), files.begin(), files.end());
	}
	updatePlaylist();
}

void ArcMusicPlayer::removeSongs() {
	updatePlaylist();
}

void ArcMusicPlayer::updatePlaylist() {}

void ArcMusicPlayer::clearPlaylist() {
	playlist.clear();
	updatePlaylist();
}

void ArcMusicPlayer::loadPlaylist() {
	Gtk::FileChooserDialog dialog("Select playlist file");
	dialog.set_transient_for(*mainWindow);

	dialog.add_button("Cancel", Gtk::RESPONSE_CANCEL);
	dialog.add_button("OK", Gtk::RESPONSE_OK);

	auto filter_text = Gtk::FileFilter::create();
	filter_text->set_name("Text files");
	filter_text->add_mime_type("text/plain");
	dialog.add_filter(filter_text);

	if (dialog.run() == Gtk::RESPONSE_OK) {
		std::ifstream file;
		file.open(dialog.get_filename());
		if (file.is_open()) {
			std::string line;
			while (std::getline(file, line)) {
				playlist.push_back(line);
			}
			file.close();
		}
	}
	updatePlaylist();
}

void ArcMusicPlayer::savePlaylist() {
	Gtk::FileChooserDialog dialog("Select save location", Gtk::FILE_CHOOSER_ACTION_SAVE);
	dialog.set_transient_for(*mainWindow);

	dialog.add_button("Cancel", Gtk::RESPONSE_CANCEL);
	dialog.add_button("OK", Gtk::RESPONSE_OK);

	if (dialog.run() == Gtk::RESPONSE_OK) {
		std::ofstream file;
		file.open(dialog.get_filename());
		if (file.is_open()) {
			for (std::vector<std::string>::iterator it = playlist.begin(); it != playlist.end(); it++) {
				file << *it << "\n";
			}
			file.close();
		}
	}
}

void ArcMusicPlayer::playpause() {
	if (!music) {
		return;
	}
	if (Mix_PausedMusic()) {
		startTicks = SDL_GetTicks() - lastPos;
		Mix_ResumeMusic();
	} else if (Mix_PlayingMusic()) {
		lastPos = (SDL_GetTicks() - startTicks) * 1000;
		Mix_PauseMusic();
	} else {
		startTicks = SDL_GetTicks();
		Mix_PlayMusic(music, 1);
	}
}

ArcMusicPlayer::ArcMusicPlayer() : playlist() {}

int main(int argc, char * argv[]){
	amp = new ArcMusicPlayer();
	int ret = amp->run(argc, argv);
	if (ret == 0) {
		Mix_HaltMusic();
		Mix_FreeMusic(amp->music);
		SDL_Quit();
	}
	return ret;
}

int ArcMusicPlayer::run(int argc, char* argv[]) {
	if (SDL_Init(SDL_INIT_AUDIO) < 0) {
		return 1;
	}
	if (Mix_Init(MIX_INIT_MP3) != MIX_INIT_MP3) {
		return 2;
	}
	Mix_OpenAudio(22050, AUDIO_S16SYS, 2, 640);
	Mix_HookMusicFinished(musicStopped);

	auto app = Gtk::Application::create(argc, argv, "org.gtkmm.examples.base");

	Glib::RefPtr<Gtk::Builder> builder = Gtk::Builder::create_from_file("ArcMusicPlayer.glade");

	builder->get_widget("window1", mainWindow);
	builder->get_widget("aboutWindow", aboutWindow);
	builder->get_widget("enableShuffle", enableShuffle);
	builder->get_widget("repeatMode", repeatMode);

	// connect button signals
	Gtk::Button *button = nullptr;

	builder->get_widget("aboutButton", button);
	button->signal_clicked().connect(sigc::mem_fun(*this, &ArcMusicPlayer::about));

	builder->get_widget("rw10Button", button);
	button->signal_clicked().connect(sigc::mem_fun(*this, &ArcMusicPlayer::rw10));

	builder->get_widget("rw30Button", button);
	button->signal_clicked().connect(sigc::mem_fun(*this, &ArcMusicPlayer::rw30));

	builder->get_widget("ff30Button", button);
	button->signal_clicked().connect(sigc::mem_fun(*this, &ArcMusicPlayer::ff30));

	builder->get_widget("ff10Button", button);
	button->signal_clicked().connect(sigc::mem_fun(*this, &ArcMusicPlayer::ff10));

	builder->get_widget("clearPlaylistButton", button);
	button->signal_clicked().connect(sigc::mem_fun(*this, &ArcMusicPlayer::clearPlaylist));

	builder->get_widget("savePlaylistButton", button);
	button->signal_clicked().connect(sigc::mem_fun(*this, &ArcMusicPlayer::savePlaylist));

	builder->get_widget("loadPlaylistButton", button);
	button->signal_clicked().connect(sigc::mem_fun(*this, &ArcMusicPlayer::loadPlaylist));

	builder->get_widget("addSongsButton", button);
	button->signal_clicked().connect(sigc::mem_fun(*this, &ArcMusicPlayer::addSongs));

	builder->get_widget("removeSongsButton", button);
	button->signal_clicked().connect(sigc::mem_fun(*this, &ArcMusicPlayer::removeSongs));

	builder->get_widget("playpauseButton", button);
	button->signal_clicked().connect(sigc::mem_fun(*this, &ArcMusicPlayer::playpause));

	// initialize interface and other stuff
	srand(time(NULL));

	return app->run(*mainWindow);
}

