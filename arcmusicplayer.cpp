//Copyright (C) 2018-9 Arc676/Alessandro Vinciguerra

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

void ArcMusicPlayer::editPlaylist() {
	playlistWindow->show();
}

void ArcMusicPlayer::volumeChanged() {
	int vol = (int)volumeSlider->get_value();
	Mix_VolumeMusic(vol);
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
	amp->dispatcher.emit();
}

void ArcMusicPlayer::checkPlaylistFinished() {
	if (playlist.size() == 0) {
		if (music) {
			Mix_FreeMusic(music);
			music = nullptr;
		}
		currentSongIndex = 0;
	}
}

void ArcMusicPlayer::playSong() {
	if (music) {
		Mix_FreeMusic(music);
	}
	const char* song = playlist[currentSongIndex].c_str();
	music = Mix_LoadMUS(song);
	// TODO: update the slider once that's added
	playpause();
	// TODO: get song duration once you figure out how to do that

	// update playlist dropdown
	isAlteringPlaylist = true;
	playlistModel->set_active(currentSongIndex);
	isAlteringPlaylist = false;

	// show notification
	if (showNotifs->get_active()) {
		int pos = playlist[currentSongIndex].find_last_of("/");
		const char* title = &song[pos + 1];
		NotifyNotification* n = notify_notification_new("Now playing", title, 0);
		notify_notification_show(n, 0);
	}
}

void ArcMusicPlayer::userChoseSong() {
	if (isAlteringPlaylist) {
		return;
	}
	currentSongIndex = playlistModel->get_active_row_number();
	playSong();
}

void ArcMusicPlayer::pickRandomSong() {
	if (playlist.size() == 0) {
		return;
	}
	currentSongIndex = rand() % playlist.size();
	playSong();
}

void ArcMusicPlayer::nextSong() {
	startTicks = SDL_GetTicks();
	if (playlist.size() > 0) {
		// if shuffle mode is active
		if (enableShuffle->get_active()) {
			playlist.erase(playlist.begin() + currentSongIndex);
			updatePlaylist();
			// stop playing if playlist is empty
			if (playlist.size() == 0) {
				currentSongIndex = 0;
			} else {
				currentSongIndex = rand() % playlist.size();
				playSong();
			}
		} else {
			// check for repeat mode
			int rep = repeatMode->get_active_row_number();
			// if playlist has NOT been exhausted or song repeat is on
			if ((int)playlist.size() > currentSongIndex + 1 || rep == 1) {
				// if song repeat is NOT on, move to next song
				if (rep != 1) {
					currentSongIndex++;
				}
				playSong();
			} else {
				// otherwise, either playlist is exhausted or repeat all is active
				currentSongIndex = 0;
				// start over if repeat all is active
				if (rep != 0) {
					playSong();
				}
			}
		}
	}
	checkPlaylistFinished();
}

void ArcMusicPlayer::prevSong() {
	if (playlist.size() == 0) {
		return;
	}
	if (SDL_GetTicks() - startTicks > 10000) {
		playSong();
		return;
	}
	if (!enableShuffle->get_active()) {
		currentSongIndex--;
		if (currentSongIndex < 0) {
			currentSongIndex = 0;
			return;
		}
		playSong();
	}
}

std::vector<std::string> ArcMusicPlayer::getPaths(bool dir = false) {
	std::vector<std::string> paths;
	Gtk::FileChooserDialog dialog(
		*mainWindow,
		dir ? "Select folder" : "Select music files",
		dir ? Gtk::FILE_CHOOSER_ACTION_SELECT_FOLDER : Gtk::FILE_CHOOSER_ACTION_OPEN
	);

	dialog.add_button("Cancel", Gtk::RESPONSE_CANCEL);
	dialog.add_button("OK", Gtk::RESPONSE_OK);

	dialog.set_select_multiple();

	if (!dir) {
		auto filter_text = Gtk::FileFilter::create();
		filter_text->set_name("Audio files");
		filter_text->add_mime_type("audio/mpeg");
		dialog.add_filter(filter_text);
	}

	if (dialog.run() == Gtk::RESPONSE_OK) {
		std::vector<std::string> files = dialog.get_filenames();
		paths.insert(paths.end(), files.begin(), files.end());
	}
	return paths;
}

void ArcMusicPlayer::addSongs() {
	appendToPlaylist(getPaths());
}

void ArcMusicPlayer::addDir() {
	#ifndef NOEXP
	std::vector<std::string> paths = getPaths(true);
	std::vector<std::string> files;
	for (auto path : paths) {
		namespace fs = std::experimental::filesystem;
		for (auto &it : fs::recursive_directory_iterator(path)) {
			if (!fs::is_directory(it.path()) &&
				it.path().extension() == ".mp3") {
				files.push_back(it.path());
			}
		}
	}
	appendToPlaylist(files);
	#endif
}

void ArcMusicPlayer::appendToPlaylist(std::vector<std::string> files) {
	if (files.size() != 0) {
		playlist.insert(playlist.end(), files.begin(), files.end());
		updatePlaylist();
	}
}

void ArcMusicPlayer::removeSongs() {
	Glib::RefPtr<Gtk::TreeSelection> selection = playlistTable->get_selection();
	selection->selected_foreach_iter(sigc::mem_fun(*this, &ArcMusicPlayer::removeSelectedSong));
	updatePlaylist();
}

void ArcMusicPlayer::removeSelectedSong(const Gtk::TreeModel::iterator& iter) {
	std::string song = (*iter).get_value(playlistColumns.song);
	int i = 0;
	for (auto it : playlist) {
		if (it.substr(it.find_last_of("/") + 1) == song) {
			break;
		}
		i++;
	}
	playlist.erase(playlist.begin() + i);
}

void ArcMusicPlayer::updatePlaylist() {
	Gtk::TreeModel::Row row;
	playlistList->clear();
	isAlteringPlaylist = true;
	playlistModel->remove_all();
	for (auto it : playlist) {
		auto song = it;
		if (!enableFullPath->get_active()) {
			song = it.substr(it.find_last_of("/") + 1);
		}
		playlistModel->append(song);
		// update table model
		row = *(playlistList->append());
		row[playlistColumns.song] = song;
	}
	playlistModel->set_active(currentSongIndex);
	isAlteringPlaylist = false;
}

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
			std::vector<std::string> input = std::vector<std::string>();
			std::string line;
			while (std::getline(file, line)) {
				input.push_back(line);
			}
			file.close();
			loadState(input);
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
			file << getWriteableState();
			file.close();
		}
	}
}

std::string ArcMusicPlayer::getWriteableState() {
	std::stringstream ss;
	if (saveState->get_active()) {
		ss << "[StateInfo]\n";
		ss << volumeSlider->get_value() << "\n";
		ss << enableShuffle->get_active() << "\n";
		ss << repeatMode->get_active_row_number() << "\n";
		ss << enableFullPath->get_active() << "\n";
		ss << "[EndStateInfo]\n";
	}
	for (auto it : playlist) {
		ss << it << "\n";
	}
	return ss.str();
}

void ArcMusicPlayer::loadState(std::vector<std::string> input) {
	for (int i = 0; i < (int)input.size(); i++) {
		std::string line = input.at(i);
		if (line == "[StateInfo]") {
			int vol = std::stoi(input.at(i + 1));
			int shuf = std::stoi(input.at(i + 2));
			int rep = std::stoi(input.at(i + 3));
			int path = std::stoi(input.at(i + 4));
			i += 5;
			line = input.at(i);
			if (line != "[EndStateInfo]") {
				return;
			}
			volumeSlider->set_value(vol);
			enableShuffle->set_active(shuf);
			repeatMode->set_active(rep);
			enableFullPath->set_active(path);
			saveState->set_active(1);
			continue;
		} else if (line == "") {
			continue;
		}
		playlist.push_back(line);
	}
}

void ArcMusicPlayer::playpause() {
	if (!music && (int)playlist.size() > 0) {
		currentSongIndex = 0;
		playSong();
		return;
	}
	if (Mix_PausedMusic()) {
		startTicks = SDL_GetTicks() - lastPos;
		Mix_ResumeMusic();
	} else if (Mix_PlayingMusic()) {
		lastPos = (SDL_GetTicks() - startTicks) * 1000;
		Mix_PauseMusic();
	} else if ((int)playlist.size() > 0){
		startTicks = SDL_GetTicks();
		Mix_PlayMusic(music, 1);
	}
}

ArcMusicPlayer::ArcMusicPlayer() : playlist(), dispatcher() {}

int main(int argc, char * argv[]){
	amp = new ArcMusicPlayer();
	int ret = amp->run(argc, argv);
	if (ret == 0) {
		Mix_HaltMusic();
		if (amp->music) {
			Mix_FreeMusic(amp->music);
		}
		Mix_CloseAudio();
		Mix_Quit();
		SDL_Quit();
	}
	return ret;
}

int ArcMusicPlayer::run(int argc, char* argv[]) {
	// initialize SDL library
	if (SDL_Init(SDL_INIT_AUDIO) < 0) {
		return 1;
	}
	Mix_OpenAudio(22050, AUDIO_S16SYS, 2, 640);
	if ((Mix_Init(MIX_INIT_MP3) & MIX_INIT_MP3) != MIX_INIT_MP3) {
		return 2;
	}
	Mix_HookMusicFinished(musicStopped);
	dispatcher.connect(sigc::mem_fun(*this, &ArcMusicPlayer::nextSong));

	// initialize notifications library
	notify_init("Arc Music Player");

	auto app = Gtk::Application::create(argc, argv, "org.gtkmm.examples.base");

	char buf[100];
	memset(buf, 0, sizeof(buf));
	size_t len = readlink("/proc/self/exe", buf, sizeof(buf));
	buf[len] = 0;
	std::string execpath = std::string(dirname(buf));
	Glib::RefPtr<Gtk::Builder> builder = Gtk::Builder::create_from_file(execpath + "/ArcMusicPlayer.glade");

	// get references to important UI elements
	builder->get_widget("window1", mainWindow);
	builder->get_widget("aboutWindow", aboutWindow);
	builder->get_widget("playlistEditor", playlistWindow);
	builder->get_widget("playlistTable", playlistTable);
	builder->get_widget("enableShuffle", enableShuffle);
	builder->get_widget("enableAutosave", enableAutosave);
	builder->get_widget("saveState", saveState);
	builder->get_widget("repeatMode", repeatMode);
	builder->get_widget("enableNotifs", showNotifs);

	// set up table model
	playlistList = Gtk::ListStore::create(playlistColumns);
	playlistTable->set_model(playlistList);
	playlistTable->append_column("Song", playlistColumns.song);
	playlistTable->get_selection()->set_mode(Gtk::SELECTION_MULTIPLE);

	builder->get_widget("enableFullPath", enableFullPath);
	enableFullPath->signal_clicked().connect(sigc::mem_fun(*this, &ArcMusicPlayer::updatePlaylist));

	builder->get_widget("volumeSlider", volumeSlider);
	volumeSlider->signal_value_changed().connect(sigc::mem_fun(*this, &ArcMusicPlayer::volumeChanged));

	builder->get_widget("playlistBox", playlistModel);
	playlistModel->signal_changed().connect(sigc::mem_fun(*this, &ArcMusicPlayer::userChoseSong));

	// connect button signals
	Gtk::Button *button = nullptr;

	builder->get_widget("aboutButton", button);
	button->signal_clicked().connect(sigc::mem_fun(*this, &ArcMusicPlayer::about));

	builder->get_widget("editPlaylistButton", button);
	button->signal_clicked().connect(sigc::mem_fun(*this, &ArcMusicPlayer::editPlaylist));

	builder->get_widget("nextSong", button);
	button->signal_clicked().connect(sigc::mem_fun(*this, &ArcMusicPlayer::nextSong));

	builder->get_widget("prevSong", button);
	button->signal_clicked().connect(sigc::mem_fun(*this, &ArcMusicPlayer::prevSong));

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

	builder->get_widget("addDirButton", button);
	button->signal_clicked().connect(sigc::mem_fun(*this, &ArcMusicPlayer::addDir));

	builder->get_widget("removeSongsButton", button);
	button->signal_clicked().connect(sigc::mem_fun(*this, &ArcMusicPlayer::removeSongs));

	builder->get_widget("playpauseButton", button);
	button->signal_clicked().connect(sigc::mem_fun(*this, &ArcMusicPlayer::playpause));

	builder->get_widget("randomSongButton", button);
	button->signal_clicked().connect(sigc::mem_fun(*this, &ArcMusicPlayer::pickRandomSong));

	// initialize interface and other stuff
	srand(time(NULL));

	std::ifstream file;
	file.open(execpath + "/.arcmusicplayerstate.playlist");
	if (file.is_open()) {
		std::vector<std::string> input = std::vector<std::string>();
		std::string line;
		while (std::getline(file, line)) {
			input.push_back(line);
		}
		file.close();
		loadState(input);
		enableAutosave->set_active(1);
		updatePlaylist();
	}

	int ret = app->run(*mainWindow);

	if (enableAutosave->get_active()) {
		std::string state = getWriteableState();
		std::ofstream file;
		file.open(execpath + "/.arcmusicplayerstate.playlist");
		if (file.is_open()) {
			file << state;
			file.close();
		}
	}

	return ret;
}

