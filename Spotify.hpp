#ifndef SPOTIFY_HPP
#define SPOTIFY_HPP

#include <iostream>
#include <vector>
using namespace std;

struct songItem{
	string title;
	string artist;
	int popularity;
	songItem* next;
};

struct playlist{
	string name;
	bool shuffle;
	vector<songItem> songs;
};

class Music {
public: 
	Music(int songTableSize);
	~Music();

	void createSongFile();
	void readSongsFile(string file);
	songItem* newSongItem(string title, string artist, int popularity);
	void addSong(string title, string artist, int popularity);

	void addPlaylist(string name);
	void toggleShuffle();
	void selectCurrent(string name);
	void addSongtoPlaylist(string title, string name);
	void deleteSong(string title, string name);
	playlist* getCurrent();
	void displayCurrent();
	void allPlaylists();
	int getNumPlaylists();

	void playSong();
	
	bool isInTable(string title);
	void printTopN(int n);
	void searchByArtist(string x);
	int getTotalSongs();
	


private:
	songItem** songTable;
	unsigned int getHash(string title);
	void updatePopularity(string title, int popularity);
	songItem* searchTable(string title);
	int tableSize;
	int totalSongs;
	vector<playlist> playlists;
	playlist* current;
};


#endif // SPOTIFY_HPP
