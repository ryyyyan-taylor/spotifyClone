#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include "Spotify.hpp"
using namespace std;

Music::Music(int size){
	songTable = new songItem*[size];
	for(int i=0; i<size; i++) songTable[i]=NULL;
	tableSize = size;
	current=NULL;
	totalSongs = 0;
}

Music::~Music(){
	for(int i=0; i<tableSize; i++){
		songItem* entry = songTable[i];
		while(entry){
			songItem* prev = entry;
			entry = entry->next;
			delete prev;
		}
	}
	playlists.clear();
	delete songTable;
}







void Music::createSongFile(){
	system("python websiteParse.py");
}

void Music::readSongsFile(string filename){
	ifstream file(filename);
	string line, line2, x, y;
	int z;
	if(file.is_open()){
		while(getline(file, line)){
			stringstream ss;
			ss<<line;
			if(line=="") continue;
			getline(ss,line2,'|');
			z = stoi(line2);
			getline(ss,line2,'|');
			x = line2;
			getline(ss,line2,'|');
			y = line2;
			addSong(x, y, z);
		}
	}
	cout<<totalSongs-1<<" songs added to archive"<<endl;
}

songItem* Music::newSongItem(string x, string y, int z){
	songItem* temp = new songItem;
	temp->title = x;
	temp->artist = y;
	temp->popularity = z;
	return temp;
}

void Music::addSong(string x, string y, int z){
	if(x=="" || y=="") return;
	if(isInTable(x)){
		updatePopularity(x, z);
		return;
	}
	int hashVal = getHash(x);
	songItem* prev = new songItem;
	prev=NULL;
	songItem* entry = new songItem;
	entry = songTable[hashVal];
	while(entry){
		prev=entry;
		entry=entry->next;
	}
	entry = newSongItem(x, y, z);
	if(prev==NULL) songTable[hashVal] = entry;
	else prev->next = entry;
	totalSongs++;
}







void Music::addPlaylist(string x){
	playlist* temp = new playlist;
	temp->name = x;
	temp->shuffle = false;
	playlists.push_back(*temp);
	if(current==NULL) current = temp;
	cout<<"Created "<<x<<endl<<endl;
}

void Music::toggleShuffle(){
	bool temp;
	cout<<endl;
	if(current==NULL){
		cout<<"No current playlist selected"<<endl<<endl;
		return;
	}
	if(current->shuffle==true){
		temp=false;
		cout<<"Playlist mode set to in-order playback"<<endl<<endl;
	}
	else{
		temp=true;
		cout<<"Playlist mode set to shuffle"<<endl<<endl;
	}
	current->shuffle = temp;
}

void Music::selectCurrent(string x){
	for(int i=0; i<playlists.size(); i++){
		if(playlists[i].name==x){
			current = &playlists[i];
			return;
		}
	}
	cout<<"Sorry, couldn't find that playlist"<<endl;
}

void Music::addSongtoPlaylist(string x, string y){
	songItem* tempSong = searchTable(x);
	if(!tempSong){
		cout<<"Sorry, couldn't find that song"<<endl;
		return;
	}
	for(int i=0; i<playlists.size(); i++){
		if(playlists[i].name==y){
			(playlists[i].songs).push_back(*tempSong);
			current = &playlists[i];
			cout<<"Added "<<x<<" to "<<y<<endl<<endl;
			return;
		}
	}
	cout<<"Sorry, couldn't find that playlist"<<endl;
	return;
}

void Music::deleteSong(string x, string y){
	for(int i=0; i<playlists.size(); i++){
		if(playlists[i].name==y){
			for(int j=0; j<playlists[i].songs.size(); j++){
				if(playlists[i].songs[j].title==x){
					playlists[i].songs.erase(playlists[i].songs.begin()+j);
					current = &playlists[i];
					cout<<"Removed "<<x<<" from "<<y<<endl<<endl;
					return;
				}
			}	
			cout<<"Sorry, couldn't find "<<x<<" in "<<y<<endl;
			return;
		}
	}
}

playlist* Music::getCurrent(){
	return current;
}

void Music::displayCurrent(){
	for(int i=0; i<current->songs.size(); i++){
		cout<<i+1<<"| "<<current->songs[i].title<<" by "<<current->songs[i].artist<<endl<<endl;
	}
}

void Music::allPlaylists(){
	for(int i=0; i<playlists.size(); i++){
		cout<<playlists[i].name<<endl;
	}
}

int Music::getNumPlaylists(){
	return playlists.size();
}






void Music::playSong(){
	songItem tempSong;
	if(current->songs.size()==0){
		cout<<"Sorry, your current playlist is empty"<<endl;
		return;
	}
	if(current->shuffle==false){
		tempSong = current->songs[0];
		current->songs.erase(current->songs.begin());
		current->songs.push_back(tempSong);
	}
	else{
		int num = rand() % current->songs.size();
		if(num==current->songs.size()) num=current->songs.size()-1;
		tempSong = current->songs[num];
	}
	cout<<"\nPlaying "<<tempSong.title<<" by "<<tempSong.artist<<endl<<endl;
}






bool Music::isInTable(string title){
	songItem* temp = searchTable(title);
	if(!temp) return false;
	else return true;
}

void Music::printTopN(int n){
	vector<songItem> songList;
	for(int i=0; i<tableSize; i++){
		songItem* temp = songTable[i];
		while(temp){
			songList.push_back(*temp);
			temp=temp->next;
		}
	}
	for(int i=0; i<songList.size(); i++){
		if(songList[i].popularity>songList[i+1].popularity)
			swap(songList[i], songList[i+1]);
	}
	if (n>=songList.size()) n=songList.size()-1;
	for(int i=0; i<n; i++){
		cout<<i+1<<"| "<<songList[i].title<<" by "<<songList[i].artist<<endl;
	}
	songList.clear();
}

void Music::searchByArtist(string x){
	vector<songItem> songsByArtist;
	for(int i=0; i<tableSize; i++){
		songItem* temp = songTable[i];
		while(temp){
			if(temp->artist==x) songsByArtist.push_back(*temp);
			temp = temp->next;
		}
	}
	if(songsByArtist.size()==0){
		cout<<"Sorry, couldn't find any songs by "<<x<<endl;
		return;
	}
	cout<<"\nSongs by "<<x<<":"<<endl;
	for(int i=0; i<songsByArtist.size(); i++){
		cout<<songsByArtist[i].title<<endl;
	}
	cout<<endl;
}

int Music::getTotalSongs(){
	return totalSongs;
}







unsigned int Music::getHash(string word){
	unsigned int hashValue = 5381;
	int length = word.length();
	for(int i=0;i<length;i++)
		hashValue=(hashValue<<5)+ hashValue + word[i];
	hashValue %= tableSize;
	return hashValue;
}

void Music::updatePopularity(string title, int pop){
	songItem* temp = searchTable(title);
	temp->popularity += pop;
}

songItem* Music::searchTable(string x){
	int hashVal = getHash(x);
	songItem* entry = songTable[hashVal];
	while(entry){
		if(entry->title==x) return entry;
		entry = entry->next;
	}
	return NULL;
}