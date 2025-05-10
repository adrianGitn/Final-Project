#include <iostream>
#include <unordered_map>
#include <string>

using namespace std;

struct CityData {
    string countryCode;
    int population;
};

struct TrieNode {
    bool isEndOfWord;
    unordered_map<char, TrieNode*> children;
    vector<CityData> cityData; //for repeats

    TrieNode() : isEndOfWord(false) {}
};

class NameTrie {
private:
    TrieNode* root;

public:
    NameTrie() {
        root = new TrieNode();
    }

    void insert(const string& name, const string& countryCode, int population) {
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

    int trieSearch(const string& name,const string& countryCode) {
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

    void printTrie(TrieNode* node = nullptr, string prefix = "", string indent = "") {
        if (!node) node = root;
        if (node->isEndOfWord)
            cout << indent << "'" << prefix << "' (END)" << endl;
        for (auto& [ch, child] : node->children) {
            printTrie(child, prefix + ch, indent + "  ");
        }
    }

    void csvTrie(const string& filename, NameTrie& trie) {
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


};
