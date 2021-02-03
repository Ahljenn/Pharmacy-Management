#include "employee.h"

void employee::search_name(HashTable<std::string, AssociativeArray<std::string, drug>, HASH_CAP>& table) {
    /* This function is an override of the user::search name
    * See user implementation for details
    * Implementation is the same aside from the private method being called before function ends.*/

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

            /* This line pushes a vector of pairs that allow the user to access a drug based on the number.*/
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
}

void employee::search_product(HashTable<std::string, AssociativeArray<std::string, drug>, HASH_CAP>& table) {
    /* This function is an override of the user::search product
    * See user implementation for details
    * Implementation is the same aside from the private method being called before function ends.*/
    
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
}

void employee::display_all(HashTable<std::string, AssociativeArray<std::string, drug>, HASH_CAP>& table) {
    /*This method displays the entire drug listing upon request
    * User class cannot access this method*/

    auto keys_of_table = table.keys();

    int num1{ 1 };
    std::cout << "Product Name" << std::setw(W + 10) << "Price/Quantity\n";
    while (!keys_of_table.empty()) {

            auto product_names = table[keys_of_table.front()].values();
            std::cout << std::setw(2) << num1 << '.' << std::left << std::setw(W) << keys_of_table.front() << '$'
                << std::setw(W2) << std::left
                << table[keys_of_table.front()][keys_of_table.front() + product_names.front().product_num].price
                << std::setw(W2) << std::right
                << table[keys_of_table.front()][keys_of_table.front() + product_names.front().product_num].quantity << '\n';
            _name_pairs.push_back(std::make_pair(keys_of_table.front(), keys_of_table.front() + product_names.front().product_num));

            product_names.pop();
            num1++;
        
        keys_of_table.pop();
    }
}

void employee::display_deliveries(HashTable<std::string, AssociativeArray<std::string, drug>, HASH_CAP>& table) {
    /*This method displays all deliveries for the entire drug listing upon request
    * This method is a variation of the "display_all" method in the employee class
    *Uses random number generation to create arbritrary amount of weeks for next arrival
    *Calculate days if number of weeks is less than 1*/

    std::default_random_engine engine{ std::random_device{}() };
    std::uniform_real_distribution<double> dist(0.10, 15.00);
    
    auto keys_of_table = table.keys();

    std::cout << std::fixed << std::setprecision(1);

    int num1{ 1 };
    std::cout << "Product Name" << std::setw(W + 10) << "Weeks/days left for next delivery:\n";
    while (!keys_of_table.empty()) {

        auto product_names = table[keys_of_table.front()].values();
        std::cout << std::setw(2) << num1 << '.' << std::left << std::setw(W) << keys_of_table.front()
            << std::setw(W2) << std::left;

        auto week = dist(engine);
        if (week <= 1.0) {
            std::cout << ceil(week * 7) << " days.\n";
        }
        else {
            std::cout << week << " weeks.\n";
        }
        product_names.pop();
        num1++;
        keys_of_table.pop();
    }
}