
/*
 * Nekolib: An extention to the standard libraries for ease of use
 * Copyright (C) 2018 Rebekah Rowe
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#pragma once

#include <algorithm>
#include <vector>

namespace neko {

// Is to be used like an unordered map, but by replacing hashing with iteration,
// it can be faster with a small amount of items Compared to boost::flat_map,
// they do not have an unordered version thus my implimentation is surprisingly
// faster.
template <typename T_Key, typename T_Value>
class UnorderedFlatMap {
   public:
    using bucket_values = std::pair<T_Key, T_Value>;
    using bucket_type = std::vector<bucket_values>;
    using iterator = typename bucket_type::iterator;
    using const_iterator = typename bucket_type::const_iterator;

    UnorderedFlatMap() {}
    UnorderedFlatMap(const bucket_type& _bucket) { this = _bucket; }
    UnorderedFlatMap(std::initializer_list<bucket_values> _bucket) {
        bucket_type tmp = _bucket;
        *this = tmp;
    }
    // vector funcs
    inline void operator=(const bucket_type& input) {
        this->clear();
        for (const auto& i : input)
            this->insert(i);  // We do this to multiple same keys
    }
    inline void clear() { this->bucket.clear(); }
    inline auto size() const { return this->bucket.size(); }
    inline auto begin() const {
        return this->bucket.begin();
    }  // TODO: fix iterators
    inline auto end() const { return this->bucket.end(); }
    // map funcs
    inline std::pair<iterator, bool> insert(const bucket_values& in) {
        auto find = this->find(in.first);
        if (find != this->end()) {  // Gotta retain normal map behaviour
            throw "Element already exists!";
            return {find, false};
        } else {
            this->bucket.push_back(in);
            return {this->bucket.end() - 1, true};
        }
    }
    inline auto emplace(T_Key key,
                        T_Value val) {  // is this really what emplace is lmao
        return this->insert({key, val});
    }
    inline void erase(const T_Key& in) {  // by value
        auto find = this->find(in);
        if (find == this->end())
            throw "Element doesnt exist";
        else
            bucket.erase(find);
    }
    inline void erase(iterator in) {  // by iterator
        bucket.erase(in);
    }
    inline void erase(const_iterator in) {  // by iterator
        bucket.erase(in);
    }
    inline T_Value& operator[](const T_Key& i) {
        auto find = this->find(i);
        if (find != this->end()) return find->second;
        return this->insert({i, T_Value()}).first->second;
    }
    inline auto find(const T_Key& in) {
        return std::find_if(this->bucket.begin(), this->bucket.end(),
                            [&](auto i) { return in == i.first; });
    }
    inline const auto find(const T_Key& in) const {
        return std::find_if(this->bucket.begin(), this->bucket.end(),
                            [&](auto i) { return in == i.first; });
    }

   private:
    bucket_type bucket;
};

// TODO, recreate boost::circular_buffer with static sizes

}  // namespace neko
