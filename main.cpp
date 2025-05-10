#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

using namespace std;
#include "cache.h"
#include "trie.cpp"

int main() {
    const string filename = "world_cities.csv";
    NameTrie trie;
    //enters every entry into trie
    ifstream file(filename);

    trie.csvTrie(filename,trie);

    string cityName;
    string countryCode;

    cout << "Select cache type:\n"
         << "1. LRU\n2. LFU\n3. FIFO\n4. Random\nChoice: ";
    int choice;
    cin >> choice;
    cin.ignore();

    auto cache = createCache(choice, 10);


    while (!(cityName == "exit") or !(countryCode == "exit")) {

        cout << "Enter country code : ";
        getline(cin, countryCode);

        if (countryCode == "exit") break;

        cout << "Enter city name: ";
        getline(cin, cityName);

        if (cityName == "exit") break;

        // searches through cache and prints info
        double pop = cache->get(countryCode, cityName);
        if (pop != -1) {
            cout << "Found in cache." << endl;
            cout << "City: " << cityName << endl;
            cout << "Country code: " << countryCode << endl;
            cout << "Population: "  <<  pop << endl;
        }
        //searches through trie if not found
        else {
            double triePop = trie.trieSearch(countryCode,cityName);
            if (triePop != -1) {
                cout << "Found in trie." << endl;
                cout << "City: " << cityName << endl;
                cout << "Country code: " << countryCode << endl;
                cout << "Population: "  <<  triePop << endl;
                // add to cache
                cache->add({countryCode, cityName, triePop});
            } else
            {
                cout << "City not found. Please check spelling and try again." << endl;
            }
        }
    }
    cout << "Program ended." <<  endl;
}
