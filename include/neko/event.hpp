
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
#include <functional>
#include <vector>

// Define NEKOLIB_USE_STANDARD_EVENT to allow use of std::function events
namespace neko {

namespace internal {

// To handle calling of events
template <typename FuncType, typename... args>
class TEvent {
   public:
    using Func = FuncType;

   private:
    std::vector<FuncType> func_pool;
    bool in_event = false;
    auto find(FuncType in) {
        return std::find(this->func_pool.begin(), this->func_pool.end(), in);
    }

   public:
    inline bool GetInEvent() { return this->in_event; }
    inline void Emit(args... a) {
        this->in_event = true;
        for (const auto& func : this->func_pool) func(a...);
        this->in_event = false;
    }
    void Listen(FuncType in) { this->func_pool.push_back(in); }
#if !defined(NEKOLIB_USE_STANDARD_EVENT)
    void Remove(FuncType in) {
        auto find = this->find(in);
        if (find != this->func_pool.end())
            this->func_pool.erase(find);
        else
            throw std::runtime_error("Listener doesnt exist");
    }
#endif
};

}  // namespace internal

// All of the varients
template <typename... args>
using FastEvent = internal::TEvent<void (*)(args...), args...>;

#if defined(NEKOLIB_USE_STANDARD_EVENT)
template <typename... args>
using Event = internal::TEvent<std::function<void(args...)>, args...>;
#endif

}  // namespace neko
