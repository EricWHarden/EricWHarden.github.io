//============================================================================
// Name        : EricHarden_CS-260_HashTable - Enhanced.cpp
// Author      : Eric Harden
// Date        : 1/29/23
// Version     : 1.2
// Copyright   : Copyright © 2017 SNHU COCE
// Course      : CS-260-X2126 Data Structures and Algorithms 20EW2
// Description : Lab 5-2: Hash Tables and Chaining
//============================================================================

#include <algorithm>
#include <climits>
#include <iostream>
#include <string> // atoi
#include <list>
#include <cstring>
#include <time.h>

#include "CSVparser.hpp"

using namespace std;

//============================================================================
// Global definitions visible to all methods and classes
//============================================================================

const unsigned int DEFAULT_SIZE = 179;

// forward declarations
double strToDouble(string str, char ch);

// define a structure to hold bid information
struct Bid {
    string bidId; // unique identifier
    string title;
    string fund;
    double amount;
    Bid() {
        amount = 0.0;
    }
};

//============================================================================
// Hash Table class definition
//============================================================================

/**
 * Define a class containing data members and methods to
 * implement a hash table with chaining.
 */
class HashTable {

private:
    static const int hashGroups = 10;
    list<pair<int, Bid>> table[hashGroups]; //List 1, Index 0, List 2, Index 1...

    // FIXME (1): Define structures to hold bids
    struct Node {
        Bid bid;
        unsigned key;
        Node* next;

        //Default constructor
        Node() {
            key = UINT_MAX;
            next = nullptr;
        }

        //Initialize with a bid
        Node(Bid aBid) : Node() {
            bid = aBid;
        }

        //Initialize with a bid and a key
        Node(Bid aBid, unsigned aKey) : Node(aBid) {
            key = aKey;
        }
    };

    //Declares nodes as type vector<Node>
    vector<Node> nodes;

public:
    //Function to check if table is empty
    bool isEmpty() const;
    //Declare hashFunction integer with key parameter
    int hashFunction(int key);
    //Function for inserting bid
    void insertBid(int key, Bid bid);
    //Function for removing bid
    void removeBid(int key);
    //Function for printing the table
    void printTable();
    //Function for searching table
    string searchTable(int key);
};


bool HashTable::isEmpty() const {
    int sum{};
    for (int i{}; i < hashGroups; i++) {
        sum += table[i].size();
    }

    if (!sum) {
        return true;
    }
    return false;
}

int HashTable::hashFunction(int key) {
    return key % hashGroups; //Example: Key -> 905, in return, this function will return 5.
}

/*
* Attempted revise of Insert function using iterators
*/
void HashTable::insertBid(int key, Bid bid) {
    int hashValue = hashFunction(key);
    auto& cell = table[hashValue];
    auto beginItr = begin(cell);
    bool keyExists = false;
    for (; beginItr != end(cell); beginItr++) {
        if (beginItr->first == key) {
            keyExists = true;
            beginItr->second = bid;
            cout << "[WARNING] Key exists. Value replaced." << endl;
            break;
        }
    }

    if (!keyExists) {
        cell.emplace_back(key, bid);
    }
    return;
}

/*
* Attempted revise of Remove function using iterators
*/
void HashTable::removeBid(int key) {
    int hashValue = hashFunction(key);
    auto& cell = table[hashValue];
    auto beginItr = begin(cell);
    bool keyExists = false;
    for (; beginItr != end(cell); beginItr++) {
        if (beginItr->first == key) {
            keyExists = true;
            beginItr = cell.erase(beginItr);
            cout << "[INFORMATION] Bid removed." << endl;
            break;
        }
    }

    if (!keyExists) {
        cout << "[WARNING] Key not found. Pair not removed."
    }
    return;
}

/*
* Attempted revise of PrintAll function using iterators
*/
void HashTable::printTable() {
    for (int i{}; i < hashGroups; i++) {
        if (table[i].size() == 0) continue;

        auto beginItr = table[i].begin();
        for (; beginItr != table[i].end(); beginItr++) {
            cout << "[INFORMATION] Key: " << beginItr->first << "Bid: " << beginItr->second << endl;
        }
    }
    return;
}

/*
* Attempted revise of Search function using iterators
*/
string HashTable::searchTable(int key) {
    int hashValue = hashFunction(key);
    auto& cell = table[hashValue];
    auto beginItr = begin(cell);
    bool keyExists = false;

    for (; beginItr != end(cell); bItr++) {
        if (beginItr->first == key) {
            keyExists = true;
            return bItr->second;
        }
    }

    if (!keyExists) {
        return "Result Not Found";
    }
}

//============================================================================
// Static methods used for testing
//============================================================================

/**
 * Display the bid information to the console (std::out)
 *
 * @param bid struct containing the bid info
 */
void displayBid(Bid bid) {
    cout << bid.bidId << ": " << bid.title << " | " << bid.amount << " | "
        << bid.fund << endl;
    return;
}

/**
 * Load a CSV file containing bids into a container
 *
 * @param csvPath the path to the CSV file to load
 * @return a container holding all the bids read
 */
void loadBids(string csvPath, HashTable* hashTable) {
    cout << "Loading CSV file " << csvPath << endl;

    // initialize the CSV Parser using the given path
    csv::Parser file = csv::Parser(csvPath);

    // read and display header row - optional
    vector<string> header = file.getHeader();
    for (auto const& c : header) {
        cout << c << " | ";
    }
    cout << "" << endl;

    try {
        // loop to read rows of a CSV file
        for (unsigned int i = 0; i < file.rowCount(); i++) {

            // Create a data structure and add to the collection of bids
            Bid bid;
            bid.bidId = file[i][1];
            bid.title = file[i][0];
            bid.fund = file[i][8];
            bid.amount = strToDouble(file[i][4], '$');

            //cout << "Item: " << bid.title << ", Fund: " << bid.fund << ", Amount: " << bid.amount << endl;

            // push this bid to the end
            hashTable->insertBid();
        }
    }
    catch (csv::Error& e) {
        std::cerr << e.what() << std::endl;
    }
}

/**
 * Simple C function to convert a string to a double
 * after stripping out unwanted char
 *
 * credit: http://stackoverflow.com/a/24875936
 *
 * @param ch The character to strip out
 */
double strToDouble(string str, char ch) {
    str.erase(remove(str.begin(), str.end(), ch), str.end());
    return atof(str.c_str());
}

/**
 * The one and only main() method
 */
int main(int argc, char* argv[]) {

    // process command line arguments
    string csvPath, searchValue;
    switch (argc) {
    case 2:
        csvPath = argv[1];
        searchValue = "98109";
        break;
    case 3:
        csvPath = argv[1];
        searchValue = argv[2];
        break;
    default:
        csvPath = "eBid_Monthly_Sales_Dec_2016.csv";
        searchValue = "98109";
    }

    // Define a timer variable
    clock_t ticks;

    // Define a hash table to hold all the bids
    HashTable* bidTable;

    if (bidTable.isEmpty()) {
        cout << "Table is empty, awaiting bids." << endl;
    }
    else {
        cout << "Table has data. Check code." << endl;
    }

    Bid bid;

    int choice = 0;
    while (choice != 9) {
        cout << "Menu:" << endl;
        cout << "  1. Load Bids" << endl;
        cout << "  2. Display All Bids" << endl;
        cout << "  3. Find Bid" << endl;
        cout << "  4. Insert Bid" << endl;
        cout << "  5. Remove Bid" << endl;
        cout << "  9. Exit" << endl;
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice) {

        case 1:
            bidTable = new HashTable();

            // Initialize a timer variable before loading bids
            ticks = clock();

            // Complete the method call to load the bids
            loadBids(csvPath, bidTable);

            // Calculate elapsed time and display result
            ticks = clock() - ticks; // current clock ticks minus starting clock ticks
            cout << "time: " << ticks << " clock ticks" << endl;
            cout << "time: " << ticks * 1.0 / CLOCKS_PER_SEC << " seconds" << endl;
            break;

        case 2:
            bidTable->printTable();
            break;

        case 3:
            ticks = clock();

            bid = bidTable->searchTable(key);

            ticks = clock() - ticks; // current clock ticks minus starting clock ticks

            if (!bid.bidId.empty()) {
                displayBid(bid);
            }
            else {
                cout << "Bid Id " << searchValue << " not found." << endl;
            }

            cout << "time: " << ticks << " clock ticks" << endl;
            cout << "time: " << ticks * 1.0 / CLOCKS_PER_SEC << " seconds" << endl;
            break;

        case 4:
            bidTable->insertBid(key, bid);
            break;

        case 5:
            bidTable->removeBid(key);
            break;
        }
    }

    cout << "Good bye." << endl;

    return 0;
}
