
#include "album.h"
#include "btree.hpp"
#include <stdio.h>

#ifndef createBT
#define createBT

bool doesExist(char const name[81]){//check if the file or directory exists
	std::ifstream tester(name);
	if(tester.good())
		return true;
	return false;
}




int main(int c, char * v[]){
	char helpStr[] = "Usage: createBT fileIN.txt treeFile.ind\n";
	if(c<3){
		printf("%s",helpStr);
		return 0;
	}
	if(!doesExist(v[1]) || !doesExist(v[2])){
		printf("%s\nProblem opening file",helpStr);
		return 0;
	}
	fstream infile(v[1], ios::in);

	printf("Hello\n");
	BTree<Album> * bt = new BTree<Album>();
	bt->writeHeader(v[2]);
	while(infile){
		Album temp;
		infile>>temp;
		if(!infile)
			break;
		cout<<temp<<endl;
		bt->insert(temp);
		bt->printTree();
		printf("\n\n\n\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n\n\n\n");
	}
	
	// bt->inorder();
	// bt->insert(a);
	// bt->inorder();
	return 0;


}




#endif
