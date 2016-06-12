#ifndef LRU_CACHE_H
#define LRU_CACHE_H

#include <list>
#include <unordered_map>
#include <assert.h>

template <typename K, typename V>
class LRUCache {

    public:
        LRUCache(uint64_t maxSize) : maxSize(maxSize) {
        };

        void put(const K& key, const V& value) {
            auto it = cacheMap.find(key);
            if (it != cacheMap.end()) {
                cacheList.erase(it->second);
                cacheMap.erase(it);
            }
            cacheList.push_front(std::make_pair(key, value));
            cacheMap.insert(std::make_pair(key, cacheList.begin()));
            clean();
        }

        V get(const K& key) {
            auto it = cacheMap.find(key);
            cacheList.splice(cacheList.begin(), cacheList, it->second);

            return it->second->second;
        }

        bool exists(const K &key) {
            return (cacheMap.count(key) > 0);
        };

        /* void put(const KEY_T &key, const VAL_T &val) {
            auto it = item_map.find(key);
            if (it != item_map.end()){
                item_list.erase(it->second);
                item_map.erase(it);
            }
            item_list.push_front(make_pair(key,val));
            item_map.insert(make_pair(key, item_list.begin()));
            clean();
        };
        bool exist(const KEY_T &key) {
            return (item_map.count(key) > 0);
        };
        VAL_T get(const KEY_T &key) {
            assert(exist(key));
            auto it = item_map.find(key);
            item_list.splice(item_list.begin(), item_list, it->second);
            return it->second->second;
        }; */

    private:
        std::list<std::pair<K, V>> cacheList;
        std::unordered_map<K, decltype(cacheList.begin())> cacheMap;
        uint64_t maxSize;

        void clean() {
            while (cacheMap.size() > maxSize) {
                auto lastIt = cacheList.end();
                lastIt--;
                cacheMap.erase(lastIt->first);
                cacheList.pop_back();
            }
        };

};

#endif // LRU_CACHE_H
