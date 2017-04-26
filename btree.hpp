
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

template<class keyType>//template should let it be of any type, so we can reuse this for more than just the album class
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
		this->numReads = 0;
		this->numWrites = 0;
		for(int i=0; i<MAX_CHILDREN; i++){
			this->root.child[i]=-1;
		}
		printf("\n\n");
	}
	//write out header and initialize filepointer
	void writeHeader (char * fileName){
		strcpy(this->treeFileName, fileName);
		this->treeFile = new fstream(fileName, ios::in | ios::out | ios::binary);
		this->treeFile->seekg(ios_base::beg);
		this->treeFile->write((char*) &this->rootAddr, sizeof(this->rootAddr));
		this->treeFile->write((char*) &this->height, sizeof(this->height));
		this->treeFile->write((char*) &this->size, sizeof(this->size));


		// putNode(this->rootAddr, this->root);
	}
	//read in header and root node to get basic b-tree set up and intialize file pointer
	void readInTree(char * fileName){
		strcpy(this->treeFileName, fileName);
		this->treeFile = new fstream(fileName, ios::in | ios::out | ios::binary);
		this->treeFile->seekg(ios_base::beg);
		this->treeFile->read((char*) &this->rootAddr, sizeof(this->rootAddr));
		this->treeFile->read((char*) &this->height, sizeof(this->height));
		this->treeFile->read((char*) &this->size, sizeof(this->size));
		this->root = getNode(this->rootAddr);

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
			BTNode<keyType> newRoot;//swap the root for the newRoot
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
			this->treeFile->seekg(ios_base::beg);
			this->treeFile->write((char*) &this->rootAddr, sizeof(this->rootAddr));//need to rewrite the header part if we change the root
			this->treeFile->write((char*) &this->height, sizeof(this->height));
			this->treeFile->write((char*) &this->size, sizeof(this->size));
		}
	}
	
	void printTree(){//prints tree (preorder)
		printTree(this->rootAddr);
	}
	
	void inorder(){//prints tree inorder
		inorder(this->rootAddr);
	}
	
	int getHeight(){
		return this->height;
	}
	
	bool search (string key){//recursive search
		return search(key, this->root, this->rootAddr);
	}
	
	void changeNodeContents(keyType &key){
		bool b = changeNodeContents(key, this->root ,this->rootAddr, key.getStringKey());//returns RecAddr as the address of node we want, and spot as the spot in contents array
		if(!b){
			printf("Invalid attempt to change, key not found\n");
		}
	}
	void totalio(int arr[2]){
		arr[0] = this->numReads;
		arr[1] = this->numWrites;
		return;
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
	int numReads;
	int numWrites;

	

//print tree, preorder
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
//print tree, inorder
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
//try to insert the node going recursuvely down until we find the right spot (will always be a leaf)
//if we need to split, split then move p and check it we need to split
//if we do, split and continue returning up
//returns key as the key that goes to the root, newNodeAddr as the new right node
//And returns split as a boolean if we need to split again as we go up
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
					currentNode.child[i+1] = newNodeAddr;
					putNode(currAddr, currentNode);
				}
			}
			else{
				// printf("\ncurrentNode node child[0]: %d\t",currentNode.child[0]);
				// printf("currentNode node child[1]: %d ",currentNode.child[1]);
				// printf("currentNode node child[2]: %d ",currentNode.child[2]);
				// printf("currentNode node child[3]: %d ",currentNode.child[3]);
				// printf("currentNode node child[4]: %d\n\n",currentNode.child[4]);
				// printf("split again\n");
				BTNode<keyType> newNode = getNode(newNodeAddr);

				// cout<<key<<"   currAddr: "<<currAddr<<"newNodeAddr: "<<newNodeAddr<<"  "<<newNode.contents[0]<<endl;
				split = true;
				int tempNewNodeAddr = newNodeAddr;
				splitNode(key, currAddr, newNodeAddr);
				// cout<<"currAddr "<<currAddr<<endl;
				currentNode = getNode(currAddr);
				// currentNode.child[currentNode.currSize+1] = tempNewNodeAddr;
				// printf("temp %d",tempNewNodeAddr);
				putNode(currAddr, currentNode);
				// printNode(currAddr);
				newNode = getNode(newNodeAddr);
				newNode.child[newNode.currSize] = tempNewNodeAddr;
				putNode(newNodeAddr, newNode);
				// printf("\ncurrentNode node child[0]: %d\t",currentNode.child[0]);
				// printf("curr child[1]: %d ",currentNode.child[1]);
				// printf("curr child[2]: %d ",currentNode.child[2]);
				// printf("curr child[3]: %d ",currentNode.child[3]);
				// printf("curr child[4]: %d\n",currentNode.child[4]);
				// printf("\nnewNode node child[0]: %d\t",newNode.child[0]);
				// printf("new child[1]: %d ",newNode.child[1]);
				// printf("new child[2]: %d ",newNode.child[2]);
				// printf("new child[3]: %d ",newNode.child[3]);
				// printf("new child[4]: %d\n\n",newNode.child[4]);

			}
		}

	}
}

//pull the node from saved place
BTNode<keyType> getNode (int recAddr){
	this->numReads++;
	// printf("getNode(%d) myNode.currSize: ",recAddr);
	BTNode<keyType> *myNode = new BTNode<keyType>();
	this->treeFile->seekp(ios_base::beg + sizeof(this->size)+sizeof(this->height)+sizeof(this->rootAddr) + recAddr*sizeof(BTNode<keyType>));
	this->treeFile->read((char*) myNode, sizeof(BTNode<keyType>));
	// printf("%d\n",myNode->currSize);
	return *myNode;
}

//print this single node
void printNode(int recAddr){
	
	BTNode<keyType> current = getNode(recAddr);
	cout<<"\t***node of size"<<current.currSize<<"***"<<endl;
	for(int i=0;i<current.currSize;i++){
		// if(current.contents[i]){
			cout<<current.contents[i]<<endl;
		// }
		// else
			// cout<<endl;	
	}

}
//put the node into its spot, may overwrite
void putNode(int recAddr, BTNode<keyType> node){
	// printf("putNode, node.currSize: %d, %d\n",recAddr,node.currSize);
	this->treeFile->seekg(ios_base::beg + sizeof(this->size)+sizeof(this->height)+sizeof(this->rootAddr) + recAddr*sizeof(BTNode<keyType>));
	this->treeFile->write((char*) &node, sizeof(BTNode<keyType>));
	this->numWrites++;
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

//Split the node we are on, return the right node's address as newNodeAddr, and the key that goes to root as key
void splitNode (keyType& key,int recAddr, int &newNodeAddr){
	printf("Now Splitting!\n");
	BTNode<keyType> currentNode = getNode(recAddr);

	
	// cout<<currentNode.contents[1 + ORDER/2]<<endl;
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
	// cout<<"key that goes to my parent is: \n"<<key<<endl;
	BTNode<keyType> newNode = currentNode;//newNode is to the right
	
	for(int i=0;i<ORDER/2;i++){
		if(i < ORDER/2 -1){
			newNode.contents[i] = currentNode.contents[i+ ORDER/2 + 1];
			// printf("i+ORDER/2+1 = %d\nright node contents[%d]: ",i+ORDER/2 +1,i);
			// cout<<newNode.contents[i]<<endl;	
		}
		
		// printf("currentNode node contents[%d]: ",i+ ORDER/2);
		// cout<<currentNode.contents[i + ORDER/2]<<endl;
		newNode.child[i] = currentNode.child[i+ ORDER/2+1];
		newNode.child[i+ ORDER/2 +1] = -1;
		currentNode.child[i+ ORDER/2 +1] = -1;
	}
	newNode.contents[ORDER/2-1] = temp2;
	// printf("\nright node contents[%d]: ",ORDER/2-1);
	// cout<<newNode.contents[ORDER/2-1]<<endl;

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
	// printf("child sent to my parent: %d\n",newNodeAddr);
	this->size++;



}
//recursively search through children to find node that matches key, or return false
//returns true if string key is found
bool search (string key, BTNode<keyType> t, int tAddr){
	if(isLeaf(t)){//base case
		for(int i=0;i<t.currSize;i++){//go through all children
			int k = strcmp(t.contents[i].getStringKey().c_str(), key.c_str());
			if(k==0)//strings are equal
				return true;
			else if(k > 0)//content's key is larger, so no need to keep searching since we passed the location it should be
				return false;
		}
	}
	else{//recursive case
		for(int i=0; i<t.currSize; i++){//go through all contents and children
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
	return false;

}
bool changeNodeContents (keyType key, BTNode<keyType> t, int tAddr, string keyString){
	if(isLeaf(t)){//base case
		for(int i=0;i<t.currSize;i++){//go through all children
			int k = strcmp(t.contents[i].getStringKey().c_str(), keyString.c_str());
			if(k==0){//strings are equal
				t.contents[i] = key;//change contents
				// cout<<t.contents[i]<<endl;
				putNode(tAddr, t);
				return true;
			}
			else if(k > 0)//content's key is larger, so no need to keep searching since we passed the location it should be
				return false;
		}
	}
	else{//recursive case
		for(int i=0; i<t.currSize; i++){//go through all contents and children
			int k = strcmp(t.contents[i].getStringKey().c_str(), keyString.c_str());
			if(k == 0){//strings are equal
				t.contents[i] = key;//change contents over
				// cout<<t.contents[i]<<endl;
				putNode(tAddr, t);
				return true;
			}
			else if(k >0)//contents key is larger
				return changeNodeContents(key, getNode(t.child[i]), t.child[i], keyString);//check child to left		
		}
		//We didn't find it in between any of the current contents, its too large so must be to the right of last one
		if(t.child[t.currSize] > 0){//if there is a child node to the right
			return changeNodeContents(key, getNode(t.child[t.currSize]), t.child[t.currSize], keyString);//check child to right
		}
		return false;
	}
	return false;

}



	
	// int search (keyType key, int recAddr = -1);

	// BTNode * FindLeaf (keyType key);
	// load a branch into memory down to the leaf with key
	// int Height 
};

#endif
