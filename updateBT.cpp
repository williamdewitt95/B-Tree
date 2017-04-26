
#include "album.h"
#include "btree.hpp"
#include <stdio.h>

#ifndef updateBT
#define updateBT

bool updatedoesExist(char const name[81]){//check if the file or directory exists
	std::ifstream tester(name);
	if(tester.good())
		return true;
	return false;
}




int main(int c, char * v[]){
	char helpStr[] = "Usage: updateBT treeFile.ind transactions.ind\n";
	if(c<3){
		printf("%s",helpStr);
		return 0;
	}
	if(!updatedoesExist(v[1]) || !updatedoesExist(v[2])){
		printf("%s\nProblem opening file",helpStr);
		return 0;
	}
	// fstream treefile(v[1], ios::in || ios::out || ios::binary);
	fstream infile(v[2], ios::in | ios::binary);

	printf("Hello\n");
	BTree<Album> * bt = new BTree<Album>();
	bt->readInTree(v[1]);
	// bt->inorder();
	bool b;
	while(infile){

		infile.peek();
		// printf("infile? %d\n",infile ? 1:0);
		if(!infile){
			break;
		}
		transaction_Album trac;
		infile.read((char*)&trac,sizeof(transaction_Album));//read in the transaction
		// printf("type: %d\n",trac.type);
		// cout<<trac.album<<endl;
		if(strcmp(trac.album.getStringKey().c_str(),"") == 0)
			break;
		switch(trac.type){
			case Insert:
				// printf("Inserting \n");
				if(!(bt->search(trac.album.getStringKey())))
					bt->insert(trac.album);//insert new album if its insert
				else
					printf("Insertion Failed - record already exists\n");

				break;
			case Search:
				printf("Searching\n");
				b = bt->search(trac.album.getStringKey());//search for the key
				if(b){
					printf("\"%s\" found\n",trac.album.getStringKey().c_str());
				}
				else{
					printf("\"%s\" not found\n",trac.album.getStringKey().c_str());
				}
				break;
			case Change:
				printf("Changing\n");
				bt->changeNodeContents(trac.album);
				break;
			default:
				printf("Error getting type of transaction\n");
				exit(0);
				break;
		}
		
	// 	Album temp;
	// 	infile>>temp;
	// 	if(!infile)
	// 		break;
	// 	// cout<<temp<<endl;
	// 	bt->insert(temp);
	// 	// bt->printTree();
	// 	// printf("\n\n\n\n\n");
		
	}
	// printf("\n\n\n\n~~~~~~~~~~~~~~B-tree of height %d~~~~~~~~~~~~~~~~~~\n\n\n\n",bt->getHeight());

	// bt->printTree();
	printf("\n\n\n\n");
	int arr[2];
	bt->totalio(arr);
	printf("\tTotal Reads: %d\n\tTotal Writes: %d\n",arr[0],arr[1]);
	// 	printf("Find Jesse Mercer|GO BEARS!|11: true");
	// else
	// 	printf("Find Jesse Mercer|GO BEARS!|11: false");
	// bt->insert(a);
	// bt->inorder();
	return 0;


}




#endif
