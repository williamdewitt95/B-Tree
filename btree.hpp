
#include <vector>
#include <set>
#include <iostream>
#include <cstring>
#include <string>
#include <cstdlib>
#include <fstream>
using namespace std;





#ifndef BTREE_H
#define BTREE_H

// template<class U>
// class keyType{
// public:
// 	U key;
// 	keyType(U s){key=s;}
// 	keyType(){}
// 	bool operator<(keyType<U> s){
// 		this->key < s.key;
// 	};
// };

// template<class T>
// class Tree{
// 	std::vector<keyType > keys;
// public:
// 	Tree(){};
// 	void add(T s){
// 		keys.push_back(s);
// 	}
// };



class Album;
const int ORDER = 5;
const int MAX_CHILDREN = ORDER;
const int MAX_CONTENTS = ORDER-1;
// typedef Album keyType;
typedef int IndexArray[MAX_CHILDREN];
template<class T>
// typedef keyType ValueArray[ORDER-1];

struct BTNode
{
        int currSize;
        T contents[MAX_CONTENTS];
        IndexArray child;
}; 

// struct Pair
// {
//         keyType element;
//         int loffset;
//         int roffset;
//         friend bool operator < (const Pair& p1, const Pair& p2)
//            { return (p1.element < p2.element);  }
// };
template<class keyType>
class BTree
{
 public:
	BTree(){
		BTNode<keyType> btn;
		this->root = btn;
		this->root.currSize=0;
		this->rootAddr = 0;
		this->height = 0;
		this->size=1;
		for(int i=0; i<MAX_CHILDREN; i++){
			this->root.child[i]=-1;
		}
		printf("\n\n");
	}
	
	void writeHeader (char * fileName){
		strcpy(this->treeFileName, fileName);
		this->treeFile = new fstream(fileName, ios::in | ios::out | ios::binary);
		// putNode(this->rootAddr, this->root);
	}
	
	void insert (keyType key){
		cout<<"Now inserting "<<key<<endl;
		if(this->root.currSize < 1){
			// printf("Initial insertion\n");
			this->root.currSize++;
			this->root.contents[0] = key;
			putNode(this->rootAddr, this->root);
			return;
		}
		// printf("insert\n");
		bool split;
		int newNodeAddr=0;
		int j;
		insert(key, this->rootAddr, split, newNodeAddr);
		// printf("\nnewNodeAddr(Right of root): %d\n", newNodeAddr);
		if(split){
			cout<<"Now Adjusting Root!"<<endl;
			this->height++;
			BTNode<keyType> newRoot;
			newRoot.currSize=1;
			newRoot.contents[0] = key;
			newRoot.child[0] = this->rootAddr;
			newRoot.child[1] = newNodeAddr;
			for(int i=2;i<MAX_CHILDREN;i++)
				newRoot.child[i]=-1;
			this->rootAddr = this->size;
			this->root = newRoot;
			this->size++;
			putNode(this->rootAddr, this->root);
			// printf("\n root\n");
			// for(int i=0;i<ORDER;i++){
			// 	printf("child[%d]: %d    ",i,this->root.child[i]);
			// }
		}
	}
	
	void reset (char * filename){

	}
	
	void close (){

	}
	
	void printTree(){
		printTree(this->rootAddr);
	}
	
	void inorder(){
		inorder(this->rootAddr);
	}
	
	void reverse(){

	}
	
	int getHeight(){
		return this->height;
	}
	
	bool search (string key){
		return search(key, this->root, this->rootAddr);
	}
	
	keyType retrieve (string key){

	}
	
	void totalio() const{

	}
	
	int countLeaves(){

	}
	// bool search (keyType key);
	// void print (ostream &);
	// void print (ostream &, int nodeAddr, int level);

	// int open (char * name);
	// int create (char * name);
 private:
	BTNode<keyType> root;
    int rootAddr;
	char treeFileName[80];
	fstream * treeFile;
	int height;
	int size;

	


void printTree (int rootAddr){
	BTNode<keyType> temp = getNode(rootAddr);
	printNode(rootAddr);
	for(int i=0; i<temp.currSize+1;i++){
		if(temp.child[i] > -1){
			// printf("temp.child[i]: %d\t",temp.child[i]);
			printTree(temp.child[i]);
		}
	}

}
void inorder (int addr){
	BTNode<keyType> temp = getNode(addr);
	for(int i=0;i<temp.currSize;i++){
		if(temp.child[i] > -1){//if we have child to left of value, print those first
			// printf("recursion\n\n\n");
			inorder(temp.child[i]);
		}
		cout<<temp.contents[i];
		printf("\n");
	}
	if(temp.child[temp.currSize] > -1){			
		// printf("recursion\n\n\n");
		inorder(temp.child[temp.currSize]);//check the last child (to the right of last content)
	}
}
void reverse (int rootAddr){

}
int findAddr (keyType key, BTNode<keyType> t, int tAddr){

}
int findpAddr(keyType key, BTNode<keyType> t, int tAddr){

}

void insert(keyType &key, int currAddr, bool &split, int &newNodeAddr){
	if(isLeaf(currAddr)){
		// printf("LEAF\t");
		BTNode<keyType> currentNode = getNode(currAddr);
		if(currentNode.currSize < MAX_CONTENTS){//there is space
			// printf("currSize < MAX_CONTENTS: %d\n",currentNode.currSize);
			split=false;
			bool b=false;
			int i;
			for(i=0; i<currentNode.currSize; i++){
				if(key < currentNode.contents[i]){//contents value is bigger, so put left 
					b=true;
					keyType temp, insertNode;
					insertNode = key;
					for(i;i<currentNode.currSize+1; i++){//continue down the array, but swapping
						temp = currentNode.contents[i];
						currentNode.contents[i] = insertNode;
						insertNode = temp;

					}
					currentNode.currSize++;
					putNode(currAddr, currentNode);	
				}
			}
			if(i <= MAX_CONTENTS-1 && !b){
				currentNode.contents[i] = key;
				currentNode.currSize++;
				putNode(currAddr, currentNode);
			}
		}
		else{
			splitNode(key, currAddr, newNodeAddr);
			split = true;
		}
	}
	else{//we split the one below us, now we might need to
		BTNode<keyType> currentNode = getNode(currAddr);
		// printf("NOT LEAF\t");
		int j;
		for(j=0;j<currentNode.currSize;j++){
			if(key < currentNode.contents[j]){
				break;
			}
		}
		insert(key, currentNode.child[j], split, newNodeAddr);
		// printf("%d, %d, %d\n",currAddr,split,newNodeAddr);
		if(split){
			// printf("split\t");
			if(currentNode.currSize < MAX_CONTENTS){
				// printf("currSize < MAX_CONTENTS: %d\n",currentNode.currSize);
				bool b = false;
				split=false;
				int i;
				for(i=0; i<currentNode.currSize; i++){
					if(key < currentNode.contents[i]){//contents value is bigger, so put left 
						b = true;
						int insertAddr, tempAddr;
						insertAddr = newNodeAddr;
						keyType temp, insertNode;
						insertNode = key;
						for(i;i<currentNode.currSize+1; i++){//continue down the array, but swapping
							temp = currentNode.contents[i];
							currentNode.contents[i] = insertNode;
							insertNode = temp;

							tempAddr = currentNode.child[i+1];
							currentNode.child[i+1] = insertAddr;
							insertAddr = tempAddr;
						}
						currentNode.currSize++;
						putNode(currAddr, currentNode);
											
					}
				}
				if(i <= MAX_CONTENTS-1 && !b){
					currentNode.contents[i] = key;
					currentNode.currSize++;
					putNode(currAddr, currentNode);
				}
			}
			else{
				split = true;
				splitNode(key, currAddr, newNodeAddr);
			}
		}

	}
}


BTNode<keyType> getNode (int recAddr){
	// printf("getNode(%d) myNode.currSize: ",recAddr);
	BTNode<keyType> *myNode = new BTNode<keyType>();
	this->treeFile->seekp(ios_base::beg + recAddr*sizeof(BTNode<keyType>));
	this->treeFile->read((char*) myNode, sizeof(BTNode<keyType>));
	// printf("%d\n",myNode->currSize);
	return *myNode;
}
void printNode(int recAddr){
	
	BTNode<keyType> current = getNode(recAddr);
	cout<<"\tNode of size "<<current.currSize<<endl;
	for(int i=0;i<current.currSize;i++){
		// if(current.contents[i]){
			cout<<current.contents[i]<<endl;
		// }
		// else
			// cout<<endl;	
	}

}
void placeNode (keyType k,int currAddr,int oneAddr,int twoAddr){

}
void putNode(int recAddr, BTNode<keyType> node){
	// printf("putNode, node.currSize: %d, %d\n",recAddr,node.currSize);
	this->treeFile->seekp(ios_base::beg + recAddr*sizeof(BTNode<keyType>));
	this->treeFile->write((char*) &node, sizeof(BTNode<keyType>));
}
//Check if the given node is a leaf
bool isLeaf (int recAddr){
	return isLeaf(getNode(recAddr));
}
//Check if the given node is a leaf
//
bool isLeaf(BTNode<keyType> t){
	// printf("isLeaf, %d\n",t.currSize);
	for(int i=0;i<t.currSize+1;i++){//one more than current size since you can one more link than items contained
		// printf("%d, ",i);
		if(t.child[i] > -1){
			
			return false;
		}
	}

	return true;
}
int countLeaves(int recAddr){

}
void adjRoot (keyType rootElem, int oneAddr, int twoAddr){

}
void splitNode (keyType& key,int recAddr, int &newNodeAddr){
	printf("Now Splitting!\n");
	BTNode<keyType> currentNode = getNode(recAddr);
	
	keyType temp1;
	keyType temp2 = key;
	for(int i=0; i<MAX_CONTENTS;i++){
		if(key < currentNode.contents[i]){//key is smaller, insert now
			temp1 = currentNode.contents[i];
			currentNode.contents[i] = temp2;
			temp2 = temp1;
		}
	}
	key = currentNode.contents[ORDER/2];
	BTNode<keyType> newNode = currentNode;//newNode is to the right
	for(int i=0;i<ORDER/2;i++){
		newNode.contents[i] = currentNode.contents[i+ ORDER/2 +1];
		newNode.child[i] = currentNode.child[i+ ORDER/2 +1];
		newNode.child[i+ ORDER/2 +1] = -1;
		currentNode.child[i+ ORDER/2 +1] = -1;
	}

	// printf("\nnewNode\n");
	// for(int i=0;i<ORDER;i++){
	// 	printf("child[%d]: %d    ",i,newNode.child[i]);
	// }
	// printf("\ncurrentNode\n");
	// for(int i=0;i<ORDER;i++){
	// 	printf("child[%d]: %d    ",i,currentNode.child[i]);
	// }
	currentNode.currSize = ORDER/2;//set current size
	newNode.currSize = ORDER/2;
	newNode.child[ORDER/2] = -1;//remove the extra child from the right side
	putNode(this->size, newNode);
	putNode(recAddr, currentNode);

	newNodeAddr = this->size;
	printf("newNodeAddr: %d\n",newNodeAddr);
	this->size++;



}
//recursively search through children to find node that matches key, or return false
//returns true if string key is found
bool search (string key, BTNode<keyType> t, int tAddr){
	if(isLeaf(t)){
		for(int i=0;i<t.currSize;i++){//go through all children
			int k = strcmp(t.contents[i].getStringKey().c_str(), key.c_str());
			if(k==0)//strings are equal
				return true;
			else if(k > 0)//content's key is larger, so no need to keep searching since we passed the location it should be
				return false;
		}
	}
	else{
		for(int i=0; i<t.currSize; i++){//go through all children
			int k = strcmp(t.contents[i].getStringKey().c_str(), key.c_str());
			if(k == 0)//strings are equal
				return true;
			else if(k >0)//contents key is larger
				return search(key, getNode(t.child[i]), t.child[i]);//check child to left		
		}
		//We didn't find it in between any of the current contents, its too large so must be to the right of last one
		if(t.child[t.currSize] > 0){//if there is a child node to the right
			return search(key, getNode(t.child[t.currSize]), t.child[t.currSize]);//check child to right
		}
		return false;
	}

}



	
	// int search (keyType key, int recAddr = -1);

	// BTNode * FindLeaf (keyType key);
	// load a branch into memory down to the leaf with key
	// int Height 
};

#endif
