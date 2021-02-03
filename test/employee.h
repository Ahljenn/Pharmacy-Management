#pragma once
#include "customer.h"
#include <random>

class employee : public customer{
	/*
	*This class inherits from customer
	* Protected members, revenue and name pairs can be access from this derived class.
	*/
public:
	employee() = default;
	void search_name(HashTable<std::string, AssociativeArray<std::string, drug>, HASH_CAP>&); //inherited from customer
	void search_product(HashTable<std::string, AssociativeArray<std::string, drug>, HASH_CAP>&); //inherited from customer
	void display_all(HashTable<std::string, AssociativeArray<std::string, drug>, HASH_CAP>&);
	void display_deliveries(HashTable<std::string, AssociativeArray<std::string, drug>, HASH_CAP>& table);
};