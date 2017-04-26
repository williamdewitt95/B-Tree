
#include "album.h"
#include "btree.hpp"
#include <iostream>
#include <cstring>
#include <string.h>
#include <cstdlib>
#include <fstream>

#ifndef transactionWriter
#define transactionWriter

bool transactiondoesExist(char const name[81]){//check if the file or directory exists
	std::ifstream tester(name);
	if(tester.good())
		return true;
	return false;
}

int main(int c, char* v[] ){

	char helpStr[] = "Usage: transactionWrite transactions.txt transactionsOUT.ind\n";
	if(c<3){
		printf("%s",helpStr);
		return 0;
	}
	if(!transactiondoesExist(v[1]) || !transactiondoesExist(v[2])){
		printf("%s\nProblem opening file\n",helpStr);
		return 0;
	}
	fstream inFile(v[1], ios::in);
	fstream outFile(v[2], ios::out | ios::binary);
	char transaction_type;
	transaction_Album trac;

	while(inFile){
		transaction_type = inFile.get();
		// printf("transaction_type: %c\n",transaction_type);
		inFile.get();
		switch(transaction_type){
			case 'i':
			case 'I':
				trac.type = Insert;
				break;
			case 'c'://do both cases because we can, and I never remember which
			case 'C':
				trac.type = Change;
				break;
			case 's':
			case 'S':
				trac.type = Search;
				break;
			default:
				printf("Error getting type\n");
				exit(0);
				break;
		}
		inFile>>trac.album;
		printf("trac: %d\n",trac.type);
		cout<<trac.album<<endl;

		inFile.peek();

		outFile.write((char*)&trac, sizeof(trac));
	}



}


#endif
