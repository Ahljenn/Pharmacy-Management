#pragma once
#include <list>
#include <queue>
#include <cstdint>

template <typename K, typename V, uint32_t CAP>
class HashTable {
public:
	HashTable(int(*)(const K&) = 0); //default constructor
	double loadFactor() const { return 1.0 * siz / CAP; }
	V operator[] (const K&) const; //getter
	V& operator[] (const K&); //setter
	bool containsKey(const K&) const;  //getter
	void deleteKey(const K&); //setter
	std::queue<K> keys() const;
	int size() const { return siz; }
	int num_indexes() const { return indexes_in_use; }
	void clear();
private:
	struct Node {
		K key;
		V value;
	};
	std::list<Node> data[CAP]; //chaining
	int(*hashCode)(const K&); //pointer to hashCode function in main
	int siz;
	int indexes_in_use;
};

template<typename K, typename V, uint32_t CAP>
HashTable<K, V, CAP>::HashTable(int(*hashCode)(const K&)) {
	this->hashCode = hashCode;
	siz = 0;
	indexes_in_use = 0;
}

template <typename K, typename V, uint32_t CAP>
V HashTable<K, V, CAP>::operator[ ](const K& key) const { //getter
	//calculate wrapped index
	int index = hashCode(key) % CAP;
	if (index < 0) index += CAP;

	typename std::list<Node>::const_iterator itr;
	for (itr = data[index].cbegin(); itr != data[index].cend(); ++itr) {
		if (itr->key == key) {
			return data[index].value;
		}
	}
	return V(); //returns a dummy value
}

template <typename K, typename V, uint32_t CAP>
V& HashTable<K, V, CAP>::operator[ ](const K& key) { //setter
	//calculate wrapped index
	int index = hashCode(key) % CAP;
	if (index < 0) index += CAP;
	typename std::list<Node>::iterator itr;

	if (data[index].empty()) {
		++indexes_in_use; //count a new index used
	}
	for (itr = data[index].begin(); itr != data[index].end(); ++itr) {
		if (itr->key == key) {
			return itr->value;
		}
	}

	//hash a new index
	Node element{ key, V() };
	data[index].push_back(element);
	++siz;
	return data[index].back().value;
}

template <typename K, typename V, uint32_t CAP>
bool HashTable<K, V, CAP>::containsKey(const K& key) const { //getter
	int index = hashCode(key) % CAP;
	if (index < 0) index += CAP;

	typename std::list<Node>::const_iterator itr;
	for (itr = data[index].cbegin(); itr != data[index].cend(); ++itr) {
		if (itr->key == key) {
			break;
		}
	}
	if (itr == data[index].cend()) {
		return false;
	}
	return true;
}

template <typename K, typename V, uint32_t CAP>
void HashTable<K, V, CAP>::deleteKey(const K& key) {
	int index = hashCode(key) % CAP;
	if (index < 0) index += CAP;

	typename std::list<Node>::iterator itr;
	for (itr = data[index].begin(); itr != data[index].end(); ++itr) {
		if (itr->key == key) { //found 
			break;
		}
	}
	if (itr == data[index].end()) { //not found
		return;
	}
	else {
		data[index].erase(itr); //fix
		--siz;
	}
}

template <typename K, typename V, uint32_t CAP>
std::queue<K> HashTable<K, V, CAP>::keys() const {
	std::queue<K> key_queue;
	for (int index{ 0 }; index < CAP; ++index) {
		typename std::list<Node>::const_iterator itr;
		for (itr = data[index].cbegin(); itr != data[index].cend(); ++itr) {
			key_queue.push(itr->key);
		}
	}
	return key_queue;
}

template <typename K, typename V, uint32_t CAP>
void HashTable<K, V, CAP>::clear() {
	for (int index{ 0 }; index < CAP; ++index) {
		typename std::list<Node>::const_iterator itr_start = data[index].begin();
		typename std::list<Node>::const_iterator itr_end = data[index].end();
		data[index].erase(itr_start, itr_end);
	}
	siz = 0;
}