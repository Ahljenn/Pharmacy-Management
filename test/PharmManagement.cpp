/*
* Ahljenn Mallari, 1741792
* Jianchong Ma, 1729107
* Joohee Hong, 1752248
* Yicong Yan, 1752248
* ISO C++17 Standard (/std:c++17)
* IMPORTANT: Please run with C++17 or later. */

#define _CRT_SECURE_NO_WARNINGS
#define _CTR_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream>
#include <string>
#include <random>
#include <cstdint>
#include <cstring>
#include "customer.h"
#include "employee.h"
#include "AssociativeArray.h"
#include "HashTable.h"

int hash_code(const std::string& key) {
    return (int)key[0];
}

void generate_rand(HashTable<std::string, AssociativeArray<std::string, drug>, HASH_CAP>& table, const std::string& arg, const std::string& arg2) {
    /*This method generates random quantity, price, and selling price for each drug
    *When the drug is pushed into the hashtable while reading the file, we call this method
    *We pass the hashtable by reference as well as the arguments to access the index*/

    std::default_random_engine engine{ std::random_device{}() };
    std::uniform_int_distribution<int> dist(1, 1000); //for quantity
    std::uniform_real_distribution<float> dist2(1.0, 25.0); //for cost
    std::uniform_real_distribution<float> dist3(3.0, 10.0);//for selling price
    table[arg][arg2].quantity = dist(engine);
    table[arg][arg2].cost = dist2(engine);
    table[arg][arg2].price = table[arg][arg2].cost + dist3(engine);//cost + profit
}

int main() {

    std::ifstream inFile("Products.csv");
    HashTable<std::string, AssociativeArray<std::string, drug>, HASH_CAP> table_of_drug(hash_code);
    HashTable<std::string, AssociativeArray<std::string, drug>, HASH_CAP> table_of_ingredient(hash_code);

    if (!inFile.is_open()) {
        throw std::runtime_error("Cannot open.");
    }

    std::set<std::string> name_pills;
    std::string first_line;
    std::getline(inFile, first_line); //read first

    char* token;
    char buff[1000];
    const char* const delim{ "," };

    while (inFile.good()) {

        std::string line;
        std::getline(inFile, line);
        std::strcpy(buff, line.c_str());
        if (buff[0] == 0)continue;
        const std::string appl_no{ token = strtok(buff,delim) };
        const std::string product_no{ (token = strtok(0,delim)) ? token : "" };
        const std::string form{ (token = strtok(0,delim)) ? token : "" };
        const std::string strength{ (token = strtok(0,delim)) ? token : "" };
        const std::string drug_name{ (token = strtok(0,delim)) ? token : "" };
        const std::string ingredient{ (token = strtok(0,delim)) ? token : "" };

        std::string name_product_no{ drug_name + product_no }; //need to be unique

        // put info in tabel of ingredient------------------------------------------------------------ -
        if (ingredient.find(";") != std::string::npos)
        {
            char* ingredient_token;
            char buffer[1000];

            std::strcpy(buffer, ingredient.c_str());
            ingredient_token = strtok(buffer, ";");
            std::string temp_ingredient(ingredient_token);

            while (ingredient_token != NULL)
            {
                table_of_ingredient[temp_ingredient][name_product_no].app_num = appl_no;
                table_of_ingredient[temp_ingredient][name_product_no].product_num = product_no;
                table_of_ingredient[temp_ingredient][name_product_no].strength = strength;
                table_of_ingredient[temp_ingredient][name_product_no].name = drug_name + "\n";
                ingredient_token = strtok(0, ";");
                if (ingredient_token != NULL)
                    temp_ingredient = ingredient_token;
            }
        }
        else {
            table_of_ingredient[ingredient][name_product_no].app_num = appl_no;
            table_of_ingredient[ingredient][name_product_no].product_num = product_no;
            table_of_ingredient[ingredient][name_product_no].strength = strength;
            table_of_ingredient[ingredient][name_product_no].name = drug_name + "\n";
        }
       
        //-------------------------------------------------------------

        table_of_drug[drug_name][name_product_no].app_num = appl_no;
        table_of_drug[drug_name][name_product_no].product_num = product_no;
        table_of_drug[drug_name][name_product_no].strength = strength;
        table_of_drug[drug_name][name_product_no].name = drug_name + "\n";
        generate_rand(table_of_drug, drug_name, name_product_no);
    }

    std::cout << "1. Customer" << std::endl << "2. Employee?" << std::endl;
    std::cout << "Press q or Q to exit\n\tInput: ";
    std::string input;
    std::getline(std::cin, input);

    while (input[0] != 'Q' && input[0] != 'q') { 
        std::string choice;

        if (std::atoi(input.c_str()) == 1) { //customer
  
            customer* new_customer = new customer;

            while (toupper(choice[0]) != 'Q') {

                std::cout << "\n1. Search for product name\n"
                    << "2. Search for specific product information.\n"
                    << "3. Search drugs for particular ingredient.\n[Q] to quit.\n\tInput: ";

                std::getline(std::cin, choice);

                if (std::atoi(choice.c_str()) == 1) { 
                    new_customer->search_name(table_of_drug);
                }
                else if (std::atoi(choice.c_str()) == 2) {
                    new_customer->search_product(table_of_drug);
                }
                else if (std::atoi(choice.c_str()) == 3) {
                    new_customer->search_ingredient(table_of_ingredient);
                }
                else if (toupper(choice[0]) == 'Q') {
                    std::cout << "Ending simulation!\n";
                }
                else {
                    std::cout << "Error, press [ENTER] to try again: ";
                    std::cin.ignore();
                }
            }
            delete new_customer;
            break;
        }

        else if (std::atoi(input.c_str()) == 2) {//employee 

            employee* new_employee = new employee;
         
            while (toupper(choice[0]) != 'Q') {

                std::cout << "\n1. Display all information\n"
                    << "2. Display all deliveries\n"
                    << "3. Search product name\n"
                    << "4. Search for specific product information\n"
                    << "5. Search for drugs with particular ingredient\n"
                    << "[Q] to quit.\n\tInput: ";

                std::getline(std::cin, choice);

                if (std::atoi(choice.c_str()) == 1) {
                    new_employee->display_all(table_of_drug);
                }
                else if (std::atoi(choice.c_str()) == 2) {
                    new_employee->display_deliveries(table_of_drug);
                }
                else if (std::atoi(choice.c_str()) == 3) {
                    new_employee->search_name(table_of_drug);
                }
                else if (std::atoi(choice.c_str()) == 4) {
                    new_employee->search_product(table_of_drug);
                }
                else if (std::atoi(choice.c_str()) == 5) {
                   new_employee->search_ingredient(table_of_ingredient);
      
                }
                else if (toupper(choice[0]) == 'Q') {
                    std::cout << "Ending simulation!\n";
                }
                else {
                    std::cout << "Error, enter again: ";
                    std::getline(std::cin, choice);
                }
            }
            delete new_employee;
            break;
        }
        else {
            std::cout << "Error, enter again: ";
            std::getline(std::cin, input);
        }
        
    }
    std::cout << std::fixed << std::setprecision(3);
}