
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

#include <stdexcept>

namespace neko::plat {

constexpr bool kLinux =
#if defined(__linux__)
    true
#else
    false
#endif
    ;
constexpr bool kWindows =
#if defined(_WIN32)
    true
#else
    false
#endif
    ;
constexpr bool kApple =
#if defined(__APPLE__)
    true
#else
    false
#endif
    ;

// This is for offsets to functions in vtable on multiple platforms.
template <typename T>
constexpr T Switch(T _linux = 0, T _windows = 0, T _osx = 0) {
    if constexpr (kLinux)
        return _linux;
    else if constexpr (kWindows)
        return _windows;
    else if constexpr (kApple)
        return _osx;
    throw std::runtime_error("Unknown Platform");
}

// String corrections
template <const char*>
constexpr const char* Switch(const char* _linux = "", const char* _windows = "",
                             const char* _osx = "") {
    if constexpr (kLinux)
        return _linux;
    else if constexpr (kWindows)
        return _windows;
    else if constexpr (kApple)
        return _osx;
    throw std::runtime_error("Unknown Platform");
}

}  // namespace neko::plat
