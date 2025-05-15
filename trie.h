#ifndef TRIE_H
#define TRIE_H

#include <string>
#include <vector>
#include <unordered_map>

struct CityData {
    std::string countryCode;
    int population;
};

struct TrieNode {
    bool isEndOfWord;
    std::unordered_map<char, TrieNode*> children;
    std::vector<CityData> cityData; // For cities with same name in different countries

    TrieNode() : isEndOfWord(false) {}
};

class NameTrie {
private:
    TrieNode* root;

public:
    NameTrie();
    
    void insert(const std::string& name, const std::string& countryCode, int population);
    
    int trieSearch(const std::string& name, const std::string& countryCode);

    void printTrie(TrieNode* node = nullptr, std::string prefix = "", std::string indent = "");
    
    void csvTrie(const std::string& filename, NameTrie& trie);
    
    std::vector<std::pair<std::string, std::string>> getAllCities() const;
};

#endif // TRIE_H