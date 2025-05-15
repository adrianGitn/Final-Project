#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

using namespace std;
#include "cache.h"
#include "trie.cpp"
#include "LoadTest.cpp"
#include "timer.h"



int main() {

    const string filename = "world_cities.csv";
    NameTrie trie;
    //enters every entry into trie
    ifstream file(filename);
    trie.csvTrie(filename,trie);



    LoadTest test(&trie);
    const string testFile = "loadTestInput.csv";
    ifstream file2(testFile);

    fstream performance("performance.csv");
    performance << "Cache Structure, Cache Uses, Time(microseconds)" << endl;

    cout << "Load test or personal use? :\n"
         << "1. Test \n2. Personal use ";
    int program;
    cin >> program;
    cin.ignore();

    if (program == 1) {
        cout << "\nGenerate new queries or load existing?\n"
             << "1. Generate new\n"
             << "2. Load from file\nChoice: ";
        int queryChoice;
        cin >> queryChoice;
        cin.ignore();

        if (queryChoice == 1) {
            // Generate and save new queries
            test.generateAndSaveTestQueries(1000, testFile);
        } else {
            // Load existing queries
            if (!test.loadQueriesFromCSV(testFile)) {
                cerr << "Falling back to generating new queries\n";
                test.generateAndSaveTestQueries(1000, testFile);
            }
        }
    }


    Timer LRUTimer;
    Timer LFUTimer;
    Timer FIFOTimer;
    Timer randomTimer;

    if (program == 1) {
        const int count = 1000;
        int found = 0;


        for (int i = 1; i <= 4; i++)
        {
            found = 0;
            auto cache = createCache(i, 10);
            //selects timer class to use
            Timer* currentTimer = nullptr;
            string strategy;

            // Select timer and set strategy name
            switch (i) {
                case 1:
                    currentTimer = &LRUTimer;
                    strategy = "LRU";
                    break;
                case 2:
                    currentTimer = &LFUTimer;
                    strategy = "LFU";
                    break;
                case 3:
                    currentTimer = &FIFOTimer;
                    strategy = "FIFO";
                    break;
                case 4:
                    currentTimer = &randomTimer;
                    strategy = "Random";
                    break;
            }


            currentTimer->start();
            for (const auto& [countryCode, cityName] : test.getQueries()) { //loop for each item in input file
                cout << countryCode << " " << cityName << "\n";

                // searches through cache and prints info
                double pop = cache->get(countryCode, cityName);
                if (pop != -1) {
                    found++;
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
            currentTimer->stop();
            performance << strategy << "," << found << "," << currentTimer->get_time() << endl;

            cout << "Program ended." <<  endl;

        }
    }

    //old loop
    else {
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
}