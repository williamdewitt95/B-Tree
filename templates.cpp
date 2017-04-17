// #include <string>

// class B{
// };

// template<class T>
// class keyType{
// public:
// 	T store;
// 	keyType(T s){store=s;}
// 	bool operator<(keyType<T> s);
// };
// template<class T>
// 	bool keyType<T>::operator<(keyType<T> s){
// 	this->store < s.store;
// }

// int main(){
// 	B b;
// 	// keyType<B> a;
// 	keyType<int> c(1);
// 	c<c;
// }



#include <string>
#include <vector>

template<class U>
class keyType{
public:
	U store;
	keyType(U s){store=s;}
	bool operator<(keyType<U> s){
		this->store < s.store;
	};
};

template<class T>
class Tree{
	std::vector<keyType<T> > keys;
public:
	Tree(){};
	void add(T s){
		keys.push_back(s);
	}
};

int main(){
	Tree<int> c;
	for(int x=0; x<10; x++)c.add(x);
}
