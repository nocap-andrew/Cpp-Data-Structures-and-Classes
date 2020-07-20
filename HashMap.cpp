#include <list>
#include <stdexcept>
#include <utility>
#include <vector>
#include <algorithm>

template<class KeyType, class ValueType, class Hash = std::hash<KeyType>>
class HashMap {
public:
    using const_iterator = typename std::list<std::pair<const KeyType, ValueType>>::const_iterator;
    using iterator = typename std::list<std::pair<const KeyType, ValueType>>::iterator;

private:
    static constexpr int START = 4;
    std::list<std::pair<const KeyType, ValueType>> _data;
    Hash _hasher;
    std::vector<iterator> iters{};
    std::vector<size_t> sz;
    size_t buckets = 0;
    size_t act_size = 0;

public:
    Hash hash_function() const {
        return _hasher;
    }

    HashMap(Hash h = Hash()) : _hasher(h) {
    }

    template<class Iter>
    HashMap(Iter begin, Iter end, Hash h = Hash()) : _hasher(h) {
        while (begin != end)
            insert(*begin++);
    }

    HashMap(const HashMap &other) :
            HashMap(other.begin(), other.end(), other._hasher) {
    }

    HashMap(std::initializer_list<std::pair<const KeyType, ValueType>> init, Hash h = Hash()) :
            HashMap(init.begin(), init.end(), h) {
    }

    HashMap &operator=(const HashMap &other) {
        if (this != &other) {
            clear();
            _hasher = other._hasher;
            for (auto i : other)
                insert(i);
            return *this;
        }
        return *this;
    }

    iterator begin() {
        return _data.begin();
    }

    const_iterator begin() const {
        return _data.begin();
    }

    iterator end() {
        return _data.end();
    }

    const_iterator end() const {
        return _data.end();
    }

    size_t size() const {
        return act_size;
    }

    bool empty() const {
        return act_size == 0;
    }

    iterator find(const KeyType __key) {
        if (buckets == 0) {
            return _data.end();
        }
        auto key = _hasher(__key) % buckets;
        auto st = iters[key];
        for (size_t i = 0; i < sz[key]; ++i, ++st) {
            if (st->first == __key) {
                return st;
            }
        }
        return _data.end();
    }

    const_iterator find(const KeyType __key) const {
        if (buckets == 0) {
            return _data.end();
        }
        auto key = _hasher(__key) % buckets;
        auto st = iters[key];
        for (size_t i = 0; i < sz[key]; ++i, ++st) {
            if (st->first == __key) {
                return st;
            }
        }
        return _data.end();
    }

    void insert(std::pair<const KeyType, ValueType> p) {
        if (buckets == 0) {
            rehash();
        }
        if (find(p.first) != _data.end()) {
            return;
        }
        auto key = _hasher(p.first) % buckets;
        if (sz[key] == 0) {
            ++act_size;
            _data.insert(_data.end(), p);
            iters[key] = _data.end();
            iters[key]--;
            sz[key] = 1;
        } else {
            ++act_size;
            ++sz[key];
            _data.insert(iters[key], p);
            iters[key]--;
        }
        if (act_size * 2 >= buckets) {
            rehash();
        }
    }

    void erase(const KeyType __key) {
        auto it = find(__key);
        if (it == _data.end()) {
            return;
        }
        auto key = _hasher(__key) % buckets;
        --sz[key];
        --act_size;
        if (sz[key] == 0) {
            iters[key] = _data.end();
        } else if (it == iters[key]) {
            ++iters[key];
        }
        _data.erase(it);
    }

    void rehash() {
        if (buckets == 0) {
            iters.resize(START);
            sz.resize(START);
            buckets = START;
        } else {
            act_size = 0;
            buckets *= 2;
            sz.clear();
            sz.resize(buckets);
            iters.clear();
            iters.resize(buckets);
            std::list<std::pair<const KeyType, ValueType>> ndata;
            swap(ndata, _data);
            for (auto i : ndata) {
                insert(i);
            }
            ndata.clear();
        }
    }

    ValueType &operator[](const KeyType __key) {
        auto it = find(__key);
        if (it != _data.end()) {
            return it->second;
        }
        insert({__key, ValueType()});
        return find(__key)->second;
    }

    const ValueType &at(const KeyType __key) const {
        auto it = find(__key);
        if (it == _data.end()) {
            throw std::out_of_range("OUT OF RANGE");
        }
        return it->second;
    }

    void clear() {
        sz.clear();
        iters.clear();
        _data.clear();
        act_size = 0;
        buckets = 0;
    }
};
