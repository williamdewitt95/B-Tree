
#include "album.h"

// const int UpTo = 50;
typedef char String[UpTo];

Album::Album(){
	strcpy(this->Key, "");
	strcpy(this->Artist, "");
	strcpy(this->Title, "");


}

Album::Album(String Key, String artist,
                String title){

	strcpy(this->Key, Key);
	strcpy(this->Artist, artist);
	strcpy(this->Title, title);
	
}
Album::Album(String Key){

	strcpy(this->Key, Key);
	
	
}


Album::Album(const Album& otherAlbum){

	strcpy(this->Key, otherAlbum.Key);
	strcpy(this->Artist, otherAlbum.Artist);
	strcpy(this->Title, otherAlbum.Title);

}

Album & Album::operator=(const Album& otherAlbum){
	strcpy(this->Key, otherAlbum.Key);
	strcpy(this->Artist, otherAlbum.Artist);
	strcpy(this->Title, otherAlbum.Title);
	return *this;
}

bool Album::operator < (const Album& b){
	if(atoi(this->Key) < atoi(b.Key))
		return true;
	return false;
}
istream & operator >> (istream & stream, Album &C){
	stream.getline(C.Key, UpTo);
	stream.getline(C.Artist, UpTo);
	stream.getline(C.Title, UpTo);
	return stream;
}
ostream & operator << (ostream & stream, Album &C){
	stream<<C.Key;
	stream<<'|';
	stream<<C.Artist;
	stream<<'|';
	stream<<C.Title;
	return stream;
}
string Album::getStringKey (){
	return string(this->Key);
}
 int Album::recordSize(){
 	return sizeof(this);
 }