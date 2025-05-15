#include <iostream>
#include <unordered_map>
#include <string>
#include "trie.h"

using namespace std;




NameTrie::NameTrie() : root(new TrieNode()) {}


void NameTrie::insert(const std::string& name, const std::string& countryCode, int population) {
    TrieNode* node = root;
    for (char c : name) {
        c = tolower(c); // Case-insensitive
        if (node->children.count(c) == 0) {
            node->children[c] = new TrieNode();
        }
        node = node->children[c];
    }
    node->isEndOfWord = true;
    node->cityData.push_back({countryCode, population});
}

int NameTrie::trieSearch(const string& name,const string& countryCode) {
    TrieNode* node = root;
    for (char c : name) {
        c = tolower(c);
        if (node->children.count(c) == 0)
            return -1;
        node = node->children[c];
    }

        //returns the correct
        if (node->isEndOfWord) {
            for (const auto& data : node->cityData) {
                if (data.countryCode == countryCode) {
                    return data.population;
                }
            }
            return -1;
        }
        return -1;
    }

    void NameTrie::printTrie(TrieNode* node, string prefix, string indent) {
        if (!node) node = root;
        if (node->isEndOfWord)
            cout << indent << "'" << prefix << "' (END)" << endl;
        for (auto& [ch, child] : node->children) {
            printTrie(child, prefix + ch, indent + "  ");
        }
    }

    void NameTrie::csvTrie(const string& filename, NameTrie& trie) {
        ifstream file(filename);
        if (!file.is_open()) {
            cerr << "Error opening file: " << filename << endl;
            return;
        }

        string line;
        getline(file, line); // skip header

        while (getline(file, line)) {
            stringstream ss(line);
            string cityName, countryCode, populationStr;

            getline(ss, cityName, ',');
            getline(ss, countryCode, ',');
            getline(ss, populationStr, ',');

            try {
                int population = stoi(populationStr);
                trie.insert(cityName, countryCode, population);
            } catch (const exception& e) {
                cerr << "Error parsing line: " << line << " - " << e.what() << endl;
            }
        }
    }

std::vector<std::pair<std::string, std::string>> NameTrie::getAllCities() const {
    std::vector<std::pair<std::string, std::string>> cities;
    std::function<void(TrieNode*, std::string)> collect;

    collect = [&](TrieNode* node, std::string prefix) {
        if (!node) return;

        if (node->isEndOfWord) {
            for (const auto& data : node->cityData) {
                // Changed order: cityName first, then countryCode
                cities.emplace_back(prefix, data.countryCode);
            }
        }

        for (const auto& child : node->children) {
            collect(child.second, prefix + child.first);
        }
    };

    collect(root, "");
    return cities;
}

