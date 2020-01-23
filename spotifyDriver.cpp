#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <unistd.h>
#include "Spotify.hpp"
using namespace std;

bool ifNumber(string input){
    int numNums = 0;
    if(input=="") return false;
    for(int i=0;i<input.length();i++){
        if(isdigit(input[i])) numNums++;
    }
    if(numNums == input.length()) return true;
    else return false;
}

void genMenu(playlist* current){
	cout<<"\n---Main Menu---"<<endl;
	if(current){
		cout<<"--Current playlist: "<<current->name<<" in playback mode: ";
		if(current->shuffle==true) cout<<"shuffle--"<<endl;
		else cout<<"in-order--"<<endl;
	}
	cout<<"------------------------------"<<endl;
	cout<<"1. Playlist Management"<<endl;
	cout<<"2. Display, search, and add songs"<<endl;
	cout<<"3. Play next song from your selected playlist"<<endl;
	cout<<"4. Quit"<<endl;
}

void playlistMenu(playlist* current){
	cout<<"\n---Playlist Management---"<<endl;
	if(current){
		cout<<"--Current playlist: "<<current->name<<" in playback mode: ";
		if(current->shuffle==true) cout<<"shuffle--"<<endl;
		else cout<<"in-order--"<<endl;
	}
	else cout<<"-No current playlist-"<<endl;
	cout<<"------------------------------"<<endl;
	cout<<"1. Toggle Shuffle on or off"<<endl;
	cout<<"2. Add or remove a song from current playlist"<<endl;
	cout<<"3. Select current playlist"<<endl;
	cout<<"4. Display current playlist"<<endl;
	cout<<"5. Show all playlists"<<endl;
	cout<<"6. Create new playlist"<<endl;
	cout<<"7. Back to main menu"<<endl;
}

void songsMenu(){
	cout<<"\n---Songs Tools---"<<endl;
	cout<<"------------------------------"<<endl;
	cout<<"1. Display full list of songs"<<endl;
	cout<<"2. Display the most popular songs"<<endl;
	cout<<"3. Search songs by artist"<<endl;
	cout<<"4. Add a song to current playlist"<<endl;
	cout<<"5. Back to main menu"<<endl;
}

int main(){
	cout<<"\n---Welcome to Spotify! Please wait as we load the song archive---\n"<<endl;
	Music spotify(100);
	spotify.createSongFile();
	spotify.readSongsFile("songs.txt");
	string genChoice, menuChoice, input, in;
	while(true){
		top:
		genMenu(spotify.getCurrent());
		getGenChoice:
		getline(cin,genChoice);
		if(ifNumber(genChoice)==false){
			cout<<"Please select one of the menu options on screen"<<endl;
			goto getGenChoice;
		}
		switch(stoi(genChoice)){
			// Playlist Management
			case 1:
				playTop:
				playlistMenu(spotify.getCurrent());
				s1:
				getline(cin,menuChoice);
				if(ifNumber(menuChoice)==false){
					cout<<"Please choose one of the menu options on screen"<<endl;
					goto s1;
				}
				switch(stoi(menuChoice)){
					// Shuffle on or off
					case 1:
						spotify.toggleShuffle();
						goto playTop;
						break;

					// Add or remove song from current
					case 2:
						if(!spotify.getCurrent()){
							cout<<"\nSorry, you have no current playlist"<<endl<<endl;
							goto playTop;
						}
						cout<<"Would you like to 1. Add a songs or 2. remove a song?"<<endl;
						manageChoice:
						getline(cin,in);
						if(in=="1"){
							cout<<"\nWhat song would you like to add? (need only type the title)"<<endl;
							getline(cin, input);
							spotify.addSongtoPlaylist(input, (spotify.getCurrent())->name);
						}
						else if(in=="2"){
							if(spotify.getCurrent()->songs.size()==0){
								cout<<"Sorry, your current playlist is empty"<<endl<<endl;
								goto playTop;
							}
							cout<<"What song would you like to remove? (need only type the title)"<<endl;
							getline(cin, input);
							spotify.deleteSong(input, spotify.getCurrent()->name);

						}
						else{
							cout<<"Please choose to either 1. Add or 2. Remove a song"<<endl;
							goto manageChoice;
						}
						goto playTop;
						break;

					// Choose current
					case 3:
						if(spotify.getNumPlaylists()==0){
							cout<<"\nSorry, you haven't created any playlists yet"<<endl<<endl;
							goto playTop;
						}
						cout<<"What playlist would you like to set to current?"<<endl;
						getline(cin, input);
						spotify.selectCurrent(input);
						goto playTop;
						break;

					case 4:
						if(!spotify.getCurrent()){
							cout<<"\nSorry, you have no current playlist"<<endl<<endl;
							goto playTop;
						}
						else if(spotify.getCurrent()->songs.size()==0){
							cout<<"Sorry, you current playlist is empty"<<endl;
							goto playTop;
						}
						cout<<"Songs in "<<spotify.getCurrent()->name<<endl;
						spotify.displayCurrent();
						goto playTop;
						break;

					case 5:
						cout<<"\nYour playlists:"<<endl;
						spotify.allPlaylists();
						cout<<endl;
						goto playTop;
						break;

					case 6:
						cout<<"\nWhat would you like to call this new playlist?"<<endl;
						getline(cin, input);
						spotify.addPlaylist(input);
						goto playTop;
						break;

					case 7:
						goto top;
						break;

					default:
						cout<<"Please choose one of the menu options on screen"<<endl;
						goto s1;
						break;
				}
			
			case 2:
				songTop:
				songsMenu();
				s2:
				getline(cin, menuChoice);
				if(ifNumber(menuChoice)==false){
					cout<<"Please choose one of the menu options on screen"<<endl;
					goto s2;
				}
				switch(stoi(menuChoice)){
					case 1:
						spotify.printTopN(spotify.getTotalSongs());
						cout<<endl;
						goto songTop;
						break;

					case 2:
						cout<<"\nHow many songs would you like to display?"<<endl;
						s3:
						getline(cin,input);
						if(ifNumber(input)==false){
							cout<<"Please enter a number of songs"<<endl;
							goto s3;
						}
						spotify.printTopN(stoi(input));
						cout<<endl;
						goto songTop;
						break;

					case 3:
						cout<<"Enter an artist's name"<<endl;
						getline(cin,input);
						spotify.searchByArtist(input);
						goto songTop;
						break;

					case 4:
						if(!spotify.getCurrent()){
							cout<<"Sorry, you have no current playlist"<<endl;
							goto songTop;
						}
						cout<<"What song would you like to add? (need only type the title)"<<endl;
						getline(cin, input);
						spotify.addSongtoPlaylist(input, (spotify.getCurrent())->name);
						goto songTop;
						break;

					case 5:
						goto top;
						break;

					default:
						cout<<"Please choose one of the menu options on screen"<<endl;
						goto s2;
						break;
				}

			case 3:
				if(!spotify.getCurrent()){
					cout<<"Sorry, you have no current playlist"<<endl;
					goto top;
				}
				else if(spotify.getCurrent()->songs.size()==0){
					cout<<"Sorry, you current playlist is empty"<<endl;
					goto top;
				}
				spotify.playSong();
				break;

			case 4:
				cout<<"\nGoodbye!"<<endl;
				return 0;

			default:
				cout<<"Please select one of the menu options on screen"<<endl;
				goto getGenChoice;
				break;
		}
	}
}