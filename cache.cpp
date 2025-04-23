#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <list>

using namespace std;

struct city {
    string countryCode;
    string cityName;
    double population;
};

class cache {
private:
    list<city> cacheList; // list stores most recent
    unordered_map<string, list <city>::iterator> cacheMap;
    size_t capacity;

public:
    cache(size_t size) : capacity(size) {}

    double get(const string& countryCode, const string& cityName) {
        string key = countryCode + "," + cityName;
        auto const it = cacheMap.find(key);

        //cout << "troubleshoot start" << endl;
        if (it == cacheMap.end()) { // not found
            cout << "Not found in cache." << endl;
            return -1;
        }
        //cout << "troubleshoot end" << endl;

        // moves current search to front
        cacheList.splice(cacheList.begin(), cacheList, it->second);
        return it->second->population;
    }

    void add(const city& c) {
        string key = c.countryCode + "," + c.cityName;

        auto it = cacheMap.find(key);

        //reorders to front
        if (it != cacheMap.end()) {
            cacheList.splice(cacheList.begin(), cacheList, it->second);
            it->second->population = c.population;
            return;
        }
        // restructures if cache fills
        if (cacheList.size() == capacity) {
            city last = cacheList.back();
            string lastKey = last.countryCode + "," + last.cityName;
            cacheMap.erase(lastKey);
            cacheList.pop_back();
        }

        // add to list and map
        cacheList.push_front(c);
        cacheMap[key] = cacheList.begin();
    }
    double searchCityInCSV(const string& filename, const string& countryCode,
                               const string& cityName) {
        ifstream file(filename);
        if (!file.is_open()) {
            cerr << "Error opening file: " << filename << endl;
            return -1;
        }

        string line;
        getline(file, line);

        while (getline(file, line)) {
            stringstream ss(line);
            string token;

            // reads country code
            getline(ss, token, ',');
            if (token != countryCode) continue;

            //reads city name
            getline(ss, token, ',');
            if (token != cityName) continue;

            // reads population
            getline(ss, token, ',');
            return stod(token);  // translates to double and returns

        }

        return -1;
    }
};