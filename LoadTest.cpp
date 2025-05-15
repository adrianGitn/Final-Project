#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <utility>
#include <random>
#include <chrono>
#include <algorithm>
#include <numeric>
#include <iomanip>
#include "trie.h"

class LoadTest {
    std::vector<std::pair<std::string, std::string>> testQueries;
    NameTrie* trie;

    public:
    LoadTest(NameTrie* trie) : trie(trie) {}
    ~LoadTest() {}

    void generateTestQueries(int count) {
        testQueries.clear();
        std::vector<std::pair<std::string, std::string>> allCities = trie->getAllCities();

        if (allCities.empty()) {
            std::cerr << "Error: No cities loaded in trie\n";
            return;
        }

        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dist(0, allCities.size() - 1);

        // 70% random, 20% repeated, 10% invalid
        int randomQueries = count * 0.7;
        int repeatedQueries = count * 0.2;
        int invalidQueries = count - randomQueries - repeatedQueries;

        // random queries
        for (int i = 0; i < randomQueries; i++) {
            testQueries.push_back(allCities[dist(gen)]);
        }

        // repeated queries
        for (int i = 0; i < repeatedQueries; i++) {
            testQueries.push_back(testQueries[i % 10]);
        }

        // invalid queries
        for (int i = 0; i < invalidQueries; i++) {
            testQueries.emplace_back("XX", "InvalidCity_" + std::to_string(i));
        }


        std::shuffle(testQueries.begin(), testQueries.end(), gen);

        std::cout << "Generated " << testQueries.size() << " test queries:\n"
                  << "- " << randomQueries << " random\n"
                  << "- " << repeatedQueries << " repeated\n"
                  << "- " << invalidQueries << " invalid\n";
    }

    const auto& getQueries() const { return testQueries; }


    void generateAndSaveTestQueries(int count, const std::string& filename) {
        generateTestQueries(count);
        saveQueriesToCSV(filename);
    }

    void saveQueriesToCSV(const std::string& filename) const {
        std::ofstream out(filename);
        out << "CountryCode,CityName,Type\n";

        for (const auto& [country, city] : testQueries) {
            std::string type = (country == "XX") ? "Invalid" : "Valid";
            out << country << "," << city << "," << type << "\n";
        }
        std::cout << "Saved " << testQueries.size() << " queries to " << filename << "\n";
    }

    bool loadQueriesFromCSV(const std::string& filename) {
        testQueries.clear();
        std::ifstream in(filename);

        if (!in.is_open()) {
            std::cerr << "Error: Could not open " << filename << "\n";
            return false;
        }

        // skip header
        std::string line;
        std::getline(in, line);

        while (std::getline(in, line)) {
            std::istringstream iss(line);
            std::string country, city, type;

            if (std::getline(iss, country, ',') &&
                std::getline(iss, city, ',') &&
                std::getline(iss, type)) {
                testQueries.emplace_back(country, city);
            }
        }
        std::cout << "Loaded " << testQueries.size() << " queries from " << filename << "\n";
        return true;
    }

};