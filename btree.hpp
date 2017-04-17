
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
// typedef Album keyType;
typedef int IndexArray[ORDER];
template<class T>
// typedef keyType ValueArray[ORDER-1];

struct BTNode
{
        int currSize;
        T contents[ORDER-1];
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
		for(int i=0; i<ORDER; i++){
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
		insert(key, this->rootAddr, 0,0);
	}
	
	void reset (char * filename){

	}
	
	void close (){

	}
	
	void printTree(){

	}
	
	void inorder(){
		inorder(this->rootAddr);
	}
	
	void reverse(){

	}
	
	int getHeight(){

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

	


void printTree (int rootAddr){

}
void inorder (int addr){
	BTNode<keyType> temp = getNode(addr);
	for(int i=0;i<temp.currSize;i++){
		if(temp.child[i] > -1){//if we have child to left of value, print those first
			inorder(temp.child[i]);
		}
		printNode(addr);//print my value
	}
	if(temp.child[temp.currSize] > -1){
		inorder(temp.child[temp.currSize]);//check the last child (to the right of last content)
	}
}
void reverse (int rootAddr){

}
int findAddr (keyType key, BTNode<keyType> t, int tAddr){

}
int findpAddr(keyType key, BTNode<keyType> t, int tAddr){

}
void insert (keyType key, int recAddr, int oneAddr, int twoAddr){

	BTNode<keyType> currentNode;
	if(recAddr == this->rootAddr){
		// currentNode = this->root;
		if(this->root.currSize < 1){
			this->root.contents[0]=key;
			this->root.currSize++;
			putNode(recAddr, this->root);
			return;
		}
		printf("this->root.currSize: %d\n\n\n",this->root.currSize);

	}
	
	currentNode = getNode(recAddr);
	
	if(!isLeaf(currentNode)){
		printf("not leaf\n");
		for(int i=0; i<currentNode.currSize; i++){
			if(key < currentNode.contents[i]){//contents value is bigger, so go to the left
				insert(key, currentNode.child[i], oneAddr, twoAddr);
			}
		}
		insert(key, currentNode.child[currentNode.currSize], oneAddr, twoAddr);
	}
	else{//is a leaf
		printf("leaf\t");
		int i;
		bool b=false;
		for(i=0; i<currentNode.currSize; i++){
			if(key < currentNode.contents[i]){//contents value is bigger, so put left 
				printf("left ");
				if(currentNode.currSize < ORDER - 2){//node is NOT full (max size is order-1)
					b=true;
					printf("not full\n");
					keyType temp, insertNode;
					insertNode = key;
					for(i;i<currentNode.currSize+1; i++){//continue down the array, but swapping
						temp = currentNode.contents[i];
						currentNode.contents[i] = insertNode;
						insertNode = temp;
					}
					currentNode.currSize++;
					putNode(recAddr, currentNode);
				}
				else{//node IS full
					printf("full\n\n");
				}	
			}
		}
		if(i <= ORDER-2 && !b){
			printf("right, not full, i=%d\n\n",i);
			currentNode.contents[i] = key;
			currentNode.currSize++;
			putNode(recAddr, currentNode);
		}
		if(recAddr == this->rootAddr)
			// this->root = currentNode;
			printf("currentNode.currSize: %d\n\n",currentNode.currSize);
;		// printNode(recAddr);
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
void placeNode (keyType k,int recAddr,int oneAddr,int twoAddr){

}
void putNode(int recAddr, BTNode<keyType> node){
	printf("putNode, node.currSize: %d\n",node.currSize);
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
	printf("isLeaf, %d\n",t.currSize);
	for(int i=0;i<t.currSize+1;i++){//one more than current size since you can one more link than items contained
		printf("%d, ",i);
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
void splitNode (keyType& key,int recAddr,int& oneAddr,int& twoAddr){

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
