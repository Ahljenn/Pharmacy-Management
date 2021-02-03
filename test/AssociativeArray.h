#pragma once
#include <queue>

template <typename K, typename V>
class AssociativeArray {
public:
	AssociativeArray() { firstNode = 0; siz = 0; }
	AssociativeArray(const AssociativeArray&); //copy constructor
	AssociativeArray<K, V>& operator=(const AssociativeArray<K, V>&);
	~AssociativeArray();
	const V& operator[] (const K&) const; //getter
	V& operator[] (const K&); //setter
	bool containsKey(const K&) const; //getter
	void deleteKey(const K&); //setter
	std::queue<K> keys() const;
	std::queue<V> values() const;
	int size() const { return siz; }
	void clear();
private:
	struct Node {
		K key;
		V value;
		Node* next;
	};
	Node* firstNode;
	int siz;
};


template <typename K, typename V>
AssociativeArray<K, V>::AssociativeArray(const AssociativeArray& original) { //copy constructor
	firstNode = nullptr;
	siz = original.siz;

	for (Node* itr = original.firstNode; itr; itr = itr->next) {
		Node* temp = new Node;
		temp->value = itr->value;
		temp->key = itr->key;
		temp->next = firstNode;
		firstNode = temp;
	}
}

template <typename K, typename V>
AssociativeArray<K, V>& AssociativeArray<K, V>::operator=(const AssociativeArray<K, V>& original) { //obj assignment
	if (this != &original) {
		while (firstNode) {
			Node* ptr = firstNode;
			firstNode = firstNode->next;
			delete ptr;
		}

		for (Node* itr = original.firstNode; itr; itr = itr->next) {
			Node* temp = new Node;
			temp->value = itr->value;
			temp->key = itr->key;
			temp->next = firstNode;
			firstNode = temp;
		}
		siz = original.siz;
	}
	return *this;

}

template <typename K, typename V>
AssociativeArray<K, V>::~AssociativeArray() { //destructor
	while (firstNode) {
		Node* itr = firstNode;
		firstNode = firstNode->next;
		delete itr;
	}
}

template <typename K, typename V>
const V& AssociativeArray<K, V>::operator[] (const K& key) const { //getter
	for (Node* itr = firstNode; itr; itr = itr->next) {
		if (itr->key == key) {
			return itr->value;
		}
	}
	return V(); //creates a dummy
}

template <typename K, typename V>
V& AssociativeArray<K, V>::operator[](const K& key) { //setter
	for (Node* itr = firstNode; itr; itr = itr->next) {
		if (itr->key == key) {
			return itr->value;
		}
	}

	//if we could not find the key
	++siz;
	Node* temp = new Node;
	temp->key = key;
	temp->value = V();
	temp->next = firstNode;
	firstNode = temp;
	return firstNode->value;
}

template <typename K, typename V>
bool AssociativeArray<K, V>::containsKey(const K& key) const {
	for (Node* itr = firstNode; itr; itr = itr->next) {
		if (itr->key == key) {
			return true;
		}
	}
	return false;
}

template <typename K, typename V>
void AssociativeArray<K, V>::deleteKey(const K& key) { //setter 
	Node* ptr, * prev;

	for (ptr = firstNode, prev = nullptr; ptr; prev = ptr, ptr = ptr->next) { //find the matching node
		if (ptr->key == key)
			break;
	}

	//if found
	if (ptr) {
		--siz;
		if (prev) {
			prev->next = ptr->next; //skips over the node at ptr
		}
		else {
			firstNode = ptr->next; //new head
		}
		delete ptr;
	}
}

template <typename K, typename V>
std::queue<K> AssociativeArray<K, V>::keys() const { //returns a queue obj
	std::queue<K> key_queue;
	for (Node* itr = firstNode; itr; itr = itr->next) {
		key_queue.push(itr->key);
	}
	return key_queue;
}
template <typename K, typename V>
std::queue<V> AssociativeArray<K, V>::values() const { //returns a queue obj
	std::queue<V> value_queue;
	for (Node* itr = firstNode; itr; itr = itr->next) {
		value_queue.push(itr->value);
	}
	return value_queue;
}
template <typename K, typename V>
void AssociativeArray<K, V>::clear() {
	while (firstNode) {
		Node* ptr = firstNode;
		firstNode = firstNode->next;
		delete ptr;
		--siz;
	}
}