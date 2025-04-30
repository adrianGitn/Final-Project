#include <iostream>
#include <string>
using namespace std;
#include "cache.h"

int main() {
    const string filename = "world_cities.csv";

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
        //searches through file if not found
        else {
            pop = searchCityInCSV(filename, countryCode, cityName);
            if (pop != -1) {
                cout << "Found in file." << endl;
                cout << "City: " << cityName << endl;
                cout << "Country code: " << countryCode << endl;
                cout << "Population: "  <<  pop << endl;
                // add to cache
                cache->add({countryCode, cityName, pop});
            } else
            {
                cout << "City not found. Please check spelling and try again." << endl;
            }
        }
    }
    cout << "Program ended." <<  endl;
}
