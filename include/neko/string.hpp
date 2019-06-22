
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

#include <string_view>
#include <vector>

namespace neko {

// Seperates multiple strings by delimiter
std::vector<std::string_view> sepstr(std::string_view, char delim = ' ');
// Seperates multiple args from one string, console argument style
std::vector<std::string_view> sepstr_q(std::string_view);

// portable case insensitive operations
namespace fuz {

const char* strstr(const char* main, const char* sub);
size_t strstr(std::string_view main, std::string_view sub);

int strcmp(const char* str1, const char* str2);
bool strcmp(std::string_view str1, std::string_view str2);

}  // namespace fuz

// Constant compiletime operations
namespace cexpr {

constexpr size_t strlen(const char* str) {
    size_t i = 0;
    while (str[i] != '\0') i++;
    return i;
}

constexpr int strcmp(const char* str1, const char* str2) {
    int size_1 = strlen(str1), size_2 = strlen(str2);
    if (size_1 != size_2) return size_2 - size_1;
    for (size_t i = 0; i < size_1; i++)
        if (str1[i] != str2[i]) return str2[i] - str1[i];
    return 0;
}

// TODO
/*constexpr const char* strstr(const char* main, const char* sub) {
    size_t main_len = strlen(main)
    size_t sub_len = strlen(sub);
    size_t diff = main_len - sub_len;
    if (diff < 0)
        return nullptr;
    size_t sub_pos = 0;
    size_t cur;
    for (size_t i = 0; i < main_len; i++) {
        if (main[i] == sub[i])
            start = 0;
    }
}*/

}  // namespace cexpr

}  // namespace neko
