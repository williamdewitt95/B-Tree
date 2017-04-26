#ifndef ALBUM
#define ALBUM

#include "btree.hpp"
using namespace std;

const int UpTo = 50;
typedef char String[UpTo];

class Album
{
   public :
       Album (const Album& otherAlbum);
       Album (String upc, String artist,
                String title);
       Album(String Key);
       Album ();
       Album & operator = (const Album& otherAlbum);
       bool operator < (const Album& b);
       friend istream & operator >> (istream & stream, Album&C);
       friend ostream & operator << (ostream & stream, Album&C);
       string getStringKey ();
       int recordSize();

   private :
       String Key, Artist, Title;
};

enum TransactionType {Insert, Change, Search};

struct transaction_Album
{
  TransactionType type;
  Album album;
};

#endif
