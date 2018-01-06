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

void ArcMusicPlayer::about() {
	aboutWindow->show();
}

void ArcMusicPlayer::rw10() {}

void ArcMusicPlayer::rw30() {}

void ArcMusicPlayer::ff10() {}

void ArcMusicPlayer::ff30() {}

void ArcMusicPlayer::addSongs() {}

void ArcMusicPlayer::removeSongs() {}

void ArcMusicPlayer::clearPlaylist() {}

void ArcMusicPlayer::loadPlaylist() {}

void ArcMusicPlayer::savePlaylist() {}

void ArcMusicPlayer::playpause() {}

int main(int argc, char * argv[]){
	ArcMusicPlayer* amp = new ArcMusicPlayer();
	return amp->run(argc, argv);
}

int ArcMusicPlayer::run(int argc, char* argv[]) {
	auto app = Gtk::Application::create(argc, argv, "org.gtkmm.examples.base");

	Glib::RefPtr<Gtk::Builder> builder = Gtk::Builder::create_from_file("ArcMusicPlayer.glade");

	Gtk::Window *window = nullptr;
	builder->get_widget("window1", window);

	builder->get_widget("aboutWindow", aboutWindow);

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

	return app->run(*window);
}

