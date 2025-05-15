#ifndef CACHE_H
#define CACHE_H

using namespace std;

struct city {
    std::string countryCode;
    std::string cityName;
    double population;
};

class Cache {
protected:
    size_t capacity;
public:
    Cache(size_t size) : capacity(size) {}
    virtual ~Cache() = default;
    virtual double get(const std::string& country, const std::string& city) = 0;
    virtual void add(const city& city) = 0;
};

class LRUcache;
class LFUcache;
class FIFOcache;
class RandomCache;

double searchCityInCSV(const std::string& filename,const std::string& countryCode,const std::string& cityName);

std::unique_ptr<Cache> createCache(int choice, size_t size);

#endif