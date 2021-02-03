#pragma once
#include <iostream>
#include <iomanip>
#include <string>
#include <set>
#include <vector>
#include <algorithm>
#include <utility>
#include "AssociativeArray.h"
#include "HashTable.h"

constexpr uint16_t W{ 80U };
constexpr uint16_t W2{ 10U };
constexpr uint32_t HASH_CAP{ 50U };

struct drug{
	std::string name;
	std::string product_num;
	std::string app_num;
	std::string strength;
	unsigned int quantity;
	float price;
	float cost;
};

class customer {
public:
	customer() = default;
	~customer() {
		std::cout << "Total revenue: $" << get_revenue() << '\n';
	}
	virtual void search_name(HashTable<std::string, AssociativeArray<std::string, drug>, HASH_CAP>&);
	virtual void search_product(HashTable<std::string, AssociativeArray<std::string, drug>, HASH_CAP>&);
	void search_ingredient(HashTable<std::string, AssociativeArray<std::string, drug>, HASH_CAP>&);
protected: //keep protected because employee needs to access these
	float _revenue{ 0.0 };
	float get_revenue() const { return _revenue; }
	std::vector<std::pair<std::string, std::string>> _name_pairs;
	std::queue <std::string> _product_queue;
private:
	void customer_interact(HashTable<std::string, AssociativeArray<std::string, drug>, HASH_CAP>&); //customer MENU
	void customer_buy(HashTable<std::string, AssociativeArray<std::string, drug>, HASH_CAP>&, const int&);
	void customer_sell(HashTable<std::string, AssociativeArray<std::string, drug>, HASH_CAP>&, const int&);
	void customer_return(HashTable<std::string, AssociativeArray<std::string, drug>, HASH_CAP>&, const int&);
};
