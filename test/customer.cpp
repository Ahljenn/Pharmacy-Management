#include "customer.h"

void customer::search_name(HashTable<std::string, AssociativeArray<std::string, drug>, HASH_CAP>& table) {
    /*This method searches the table for a specific name
    * Finds all occuring words for the given input
    * Displays the full name, price and quantity
    * The funciton will call the private class method, "customer_interact" */

    _name_pairs.clear();

    //serach for the name of a product
    std::string input;
    while (input.length() == 0) {
        std::cout << "\nSearch for a name: ";
        std::getline(std::cin, input);
    }

    //turns the string into a capital one
    for (int i = 0; i < input.length(); ++i) {
        input[i] = toupper(input[i]);
    }

    auto keys_of_table = table.keys();
    std::set<std::pair<std::string, std::string>> drug_names;

    while (!keys_of_table.empty()) {
        if (keys_of_table.front().find(input) != std::string::npos) {
            auto product_names = table[keys_of_table.front()].values();

            /* Use a set to sort the table keys*/
            drug_names.insert(std::make_pair(keys_of_table.front(), keys_of_table.front() + product_names.front().product_num));

            /* This line pushes a vector of pairs that allow the customer to access a drug based on the number.*/
            _name_pairs.push_back(std::make_pair(keys_of_table.front(), keys_of_table.front() + product_names.front().product_num));

            product_names.pop();
        }
        keys_of_table.pop();
    }

    std::sort(_name_pairs.begin(), _name_pairs.end());

    //Note: must use C++17 or later to allow for structured bindings
    for (auto [itr, i] = std::tuple{ drug_names.cbegin(), int{1} }; itr != drug_names.cend(); ++itr, ++i) {
        std::cout << '\n';
        std::cout << i << '.' << table[itr->first][itr->second].name
            << '$' << table[itr->first][itr->second].price
            << " | Amount: " << table[itr->first][itr->second].quantity << '\n';
    }
    customer_interact(table);
}

void customer::search_product(HashTable<std::string, AssociativeArray<std::string, drug>, HASH_CAP>& table) { /*Display function*/

    /* This method searches product information
    * customer MUST enter the exact, full string name
    * Displays full name, and strength. */

    _name_pairs.clear();
    std::string input;
    std::cout << "Search for the info of a product: ";
    std::getline(std::cin, input);

    //turns the string into a capital one
    for (int i = 0; i < input.length(); ++i) {
        input[i] = toupper(input[i]);
    }

    std::cout << std::endl;

    //serach for the info of a product
    std::queue<std::string> keys_of_drug_list = table[input].keys();

    int num2 = 1;

    while (!keys_of_drug_list.empty()) {
        std::cout << num2 << ".";
        std::cout << "Drug: " << table[input][keys_of_drug_list.front()].name << std::endl;
        std::cout << "Strength: " << table[input][keys_of_drug_list.front()].strength << std::endl;
        std::cout << std::endl;
        keys_of_drug_list.pop();
        num2++;
    }
    customer_interact(table);
}

void customer::customer_interact(HashTable<std::string, AssociativeArray<std::string, drug>, HASH_CAP>& table) {

    /* This method is only called by customer class
    * Control calls this method in the other class methods
    * customer can chose to buy, sell, or return drugs displayed by the program */

    if (_name_pairs.empty() && _product_queue.empty()) {
        std::cout << "Sorry, no matching drugs. Try again? [Y/N]: "; 
        std::string error_input;
        std::getline(std::cin, error_input);

        while (true) {
            if (toupper(error_input[0]) == 'Y') {
                search_name(table);
                return;
            }
            else if (toupper(error_input[0]) == 'N') {
                return;
            }
            else {
                std::cout << "Error try, again [Y/N]: ";
                std::getline(std::cin, error_input);
            }
        }
    }

    enter_drug:
    std::string drug_num;
    std::cout << "\nEnter number of drug: ";
    std::getline(std::cin, drug_num);

    if (atoi(drug_num.c_str()) <= 0 || atoi(drug_num.c_str()) > _name_pairs.size()) {
        std::cout << "Invalid request, try again? [Y/N]: ";
        std::string error_input;
        std::getline(std::cin, error_input);

        while (true) {
            if (toupper(error_input[0]) == 'Y') {
                goto enter_drug;
            }
            else if (toupper(error_input[0]) == 'N') {
                return;
            }
            else {
                std::cout << "Error try, again [Y/N]: ";
                std::getline(std::cin, error_input);
            }
        }
    }

    std::cout << "Buy, Sell, Or Return? [B,S,R]: ";
    char in;
    std::cin >> in;
    std::cin.ignore();

    while (toupper(in) != 'B' && toupper(in) != 'S' && toupper(in) != 'R') {
        std::cout << "Error, enter again:";
        std::cin >> in;
        std::cin.ignore();
    }

    switch (toupper(in)) {
    case 'B': {
        customer_buy(table, atoi(drug_num.c_str()));
        break;
    }
    case 'S': {
        customer_sell(table, atoi(drug_num.c_str()));
        break;
    }
    case 'R': {
        customer_return(table, atoi(drug_num.c_str()));
        break;
    }
    default: {
        break;
    }
    }
    std::cout << "\nPress [ENTER] to continue: ";
    std::cin.ignore();
}

void customer::customer_buy(HashTable<std::string, AssociativeArray<std::string, drug>, HASH_CAP>& table, const int& x) {

    std::string amt{ 0 };
    std::cout << "How many to buy? ";
    std::getline(std::cin, amt);

    if (atoi(amt.c_str()) >= table[_name_pairs.at(x - 1).first][_name_pairs.at(x - 1).second].quantity && table[_name_pairs.at(x - 1).first][_name_pairs.at(x - 1).second].quantity > 0) {
        std::cout << "\nPurchased all pills\t(Amount): " << table[_name_pairs.at(x - 1).first][_name_pairs.at(x - 1).second].quantity << '\n';
        std::cout << "Total cost: $" << table[_name_pairs.at(x - 1).first][_name_pairs.at(x - 1).second].quantity * table[_name_pairs.at(x - 1).first][_name_pairs.at(x - 1).second].price;
        table[_name_pairs.at(x - 1).first][_name_pairs.at(x - 1).second].quantity = 0;
    }
   else if (table[_name_pairs.at(x - 1).first][_name_pairs.at(x - 1).second].quantity == 0)
        std::cout << "This product is out of stock." << std::endl;
    else {
        table[_name_pairs.at(x - 1).first][_name_pairs.at(x - 1).second].quantity -= atoi(amt.c_str());
        std::cout << "\nPurchased " << amt << " of " <<
            table[_name_pairs.at(x - 1).first][_name_pairs.at(x - 1).second].name << '\n'
            << "Total cost: $" << static_cast<double>(table[_name_pairs.at(x - 1).first][_name_pairs.at(x - 1).second].price) * static_cast<double>(atoi(amt.c_str())) << '\n';
        _revenue += atoi(amt.c_str()) * (static_cast<double>(table[_name_pairs.at(x - 1).first][_name_pairs.at(x - 1).second].price) - static_cast<double>(table[_name_pairs.at(x - 1).first][_name_pairs.at(x - 1).second].cost));//calculate _revenue
    }
}

void customer::customer_sell(HashTable<std::string, AssociativeArray<std::string, drug>, HASH_CAP>& table, const int& x) {
    std::string amt{ 0 };
    std::cout << "\nHow many to sell? ";
    std::getline(std::cin, amt);
    table[_name_pairs.at(x - 1).first][_name_pairs.at(x - 1).second].quantity += atoi(amt.c_str());
    std::cout << "We pay you: $" << static_cast<double>(table[_name_pairs.at(x - 1).first][_name_pairs.at(x - 1).second].cost) * static_cast<double>(atoi(amt.c_str())) << '\n';//buy the drug from the customer with cost price
    _revenue -= static_cast<double>(table[_name_pairs.at(x - 1).first][_name_pairs.at(x - 1).second].cost) * static_cast<double>(atoi(amt.c_str()));
}

void customer::customer_return(HashTable<std::string, AssociativeArray<std::string, drug>, HASH_CAP>& table, const int& x) {
    std::string amt{ 0 };
    std::cout << "\nHow many to return? ";
    std::getline(std::cin, amt);
    table[_name_pairs.at(x - 1).first][_name_pairs.at(x - 1).second].quantity += atoi(amt.c_str());
    std::cout << "Money back: $" << static_cast<double>(table[_name_pairs.at(x - 1).first][_name_pairs.at(x - 1).second].price) * static_cast<double>(atoi(amt.c_str())) << '\n';
    _revenue -= atoi(amt.c_str()) * (static_cast<double>(table[_name_pairs.at(x - 1).first][_name_pairs.at(x - 1).second].price) - static_cast<double>(table[_name_pairs.at(x - 1).first][_name_pairs.at(x - 1).second].cost));
}

void customer::search_ingredient(HashTable<std::string, AssociativeArray<std::string, drug>, HASH_CAP>& table) { /*Display function*/

    /*This method searches the table for a specific name
    * Finds all occuring words for the given input
    * Displays the full name, price and quantity */

    //serach for the name of a product
    _name_pairs.clear();
    std::string input;
    while (input.length() == 0) {
        std::cout << "\nSearch for a name: ";
        std::getline(std::cin, input);
    }

    //turns the string into a capital one
    for (int i = 0; i < input.length(); ++i) {
        input[i] = toupper(input[i]);
    }
    std::cout << std::endl;

    auto keys_of_table = table.keys();

    std::cout << "Products:" << '\n';
    int count = 1;

    while (!keys_of_table.empty())
    {
        if (keys_of_table.front().find(input) != std::string::npos)
        {
            _product_queue = table[keys_of_table.front()].keys();

            while (!_product_queue.empty())
            {
                std::cout << count << ". " << table[keys_of_table.front()][_product_queue.front()].name;
                std::cout << "Strength: " << table[keys_of_table.front()][_product_queue.front()].strength << "\n\n";
                _product_queue.pop();
                count++;
            }
        }
        keys_of_table.pop();
    }
    if (count == 1) {
        std::cout << "Results not found.\n";
    }
    else {
        std::cout << "\nHere is the whole list of products along with their strengths by the desired active ingredient.\n\n";
    }
}