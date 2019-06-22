
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

namespace neko {

// Use when you have a function that doesnt need the aditional functionality of
// std::function The point of this is to be faster than std::function but retain
// its usability Credits to fission, thanks bby <3
template <typename>
class Function;

template <typename ret, typename... args>
class Function<ret(args...)> {
    using FuncPtr = ret (*)(args...);  // For std::function template like use
   public:
#if defined(_MSC_VER)  // msvc does some wierd things so we try to correct it
                       // with this
   private:
    static ret init(args... a) { return ret(); }

   public:
    Function(FuncPtr func = init) : func(func), original(func) {}
#else
    Function(FuncPtr func = [](args...) { return ret(); })
        : func(func), original(func) {}
#endif
    inline auto operator()(args... a) { return func(a...); }
    inline void operator=(FuncPtr _func) { func = _func; }

   private:
    FuncPtr func = nullptr;
    const FuncPtr original;
};

}  // namespace neko
