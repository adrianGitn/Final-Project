#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <map>
#include <random>
#include <list>
#include "cache.h"

using namespace std;

class LRUcache : public Cache {
    list<city> cacheList; // list stores most recent
    unordered_map<string, list <city>::iterator> cacheMap;
    size_t capacity;

public:
    LRUcache(size_t size) : Cache(size), capacity(size) {}

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
};

struct Node {
    string key;
    city value;
    int count;
    int timeStamp;

    Node(const string& key, const city& val, int ts)
        : key(key), value(val), count(1), timeStamp(ts) {}
};


class LFUcache : public Cache{

  public:
    int capacity;
    int curSize;
    int curTime;
    vector<Node *> cacheList;

    // Constructor to initialize values
    LFUcache(size_t capacity) : Cache(capacity) {
        this->capacity = capacity;
        curSize = 0;
        curTime = 0;
        cacheList.resize(capacity, nullptr);
    }


    // Function to get the key's value
    double get(const string& countryCode, const string& cityName) {
        string key = countryCode + "," + cityName;
        curTime++;

        for (int i = 0; i < capacity; i++) {
            if (cacheList[i] != nullptr && cacheList[i]->key == key) {
                cacheList[i]->count++;
                cacheList[i]->timeStamp = curTime;
                return cacheList[i]->value.population;
            }
        }
        return -1;
    }

    void add(const city& c) {
        string key = c.countryCode + "," + c.cityName;
        curTime++;

        if (capacity == 0)
            return;

        for (int i = 0; i < capacity; i++) {
            if (cacheList[i] != nullptr && cacheList[i]->key == key) {
                cacheList[i]->value = c;
                cacheList[i]->count++;
                cacheList[i]->timeStamp = curTime;
                return;
            }
        }

        if (curSize < capacity) {

            curSize++;
            for (int i = 0; i < capacity; i++) {
                if (cacheList[i] == nullptr) {
                    cacheList[i] = new Node(key, c, curTime);
                    return;
                }
            }
        }

        else {
            int minCnt = INT_MAX, minTime = INT_MAX, minIndex = -1;
            for (int i = 0; i < capacity; i++) {

                if (cacheList[i]->count < minCnt ||
                    (cacheList[i]->count == minCnt && cacheList[i]->timeStamp < minTime)) {
                    minCnt = cacheList[i]->count;
                    minTime = cacheList[i]->timeStamp;
                    minIndex = i;
                }
            }
            cacheList[minIndex] = new Node(key, c, curTime);
        }
    }
};

class FIFOcache : public Cache {
private:
    queue<string> order; // list stores most recent
    unordered_map<string, city> cacheMap;
    size_t capacity;

public:
    FIFOcache(size_t size) : Cache(size), capacity(size) {}

    double get(const string& country, const string& city) {
        string key = country + "," + city;
        return cacheMap.count(key) ? double(cacheMap[key].population) : -1;
    }

    void add(const city& c) {
        string key = c.countryCode + "," + c.cityName;

        if (cacheMap.size() >= capacity && !cacheMap.count(key)) {
            cacheMap.erase(order.front());
            order.pop();
        }

        if (!cacheMap.count(key)) order.push(key);
        cacheMap[key] = c;
    }
};
class RandomCache : public Cache {
private:
    vector<string> keys;
    unordered_map<string, city> cache;
    mt19937 rng;
    size_t capacity;
public:
    RandomCache(size_t size) : Cache(size), capacity(size) {
        rng.seed(random_device{}());
    }

    double get(const string& country, const string& city) {
        string key = country + "," + city;
        return cache.count(key) ? double(cache[key].population) : -1;
    }

    void add(const city& city) {
        string key = city.countryCode + "," + city.cityName;

        if (cache.size() >= capacity && !cache.count(key)) {
            uniform_int_distribution<size_t> dist(0, keys.size()-1);
            cache.erase(keys[dist(rng)]);
        }

        if (!cache.count(key)) keys.push_back(key);
        cache[key] = city;
    }
};

double searchCityInCSV(const string& filename, const string& countryCode, const string& cityName) {
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

std::unique_ptr<Cache> createCache(int choice, size_t size) {
    switch (choice) {
        case 1: return std::make_unique<LRUcache>(size);
        case 2: return std::make_unique<LFUcache>(size);
        case 3: return std::make_unique<FIFOcache>(size);
        case 4: return std::make_unique<RandomCache>(size);
        default:
            std::cerr << "Invalid choice. Using LRU as default.\n";
        return std::make_unique<LRUcache>(size);
    }
}
