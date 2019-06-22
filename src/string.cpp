
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

#include <algorithm>
#include <cstring>
#include <string>

#include <neko/platform.hpp>
#include <neko/string.hpp>

namespace neko {

std::vector<std::string_view> sepstr(std::string_view input, char delim) {
    std::vector<std::string_view> ret;
    while (!input.empty()) {
        // Discard multiple
        std::size_t non_white_start = input.find_first_not_of(delim);
        if (non_white_start == std::string_view::npos) break;
        input.remove_prefix(non_white_start);

        // Find delim
        std::size_t first_delim = input.find_first_of(delim);
        if (first_delim == std::string_view::npos) break;
        std::size_t size = first_delim;

        // act on delim
        ret.push_back(input.substr(0, first_delim - 1));
        input.remove_prefix(first_delim);
    }
    if (!input.empty()) ret.push_back(input);
    return ret;
}
std::vector<std::string_view> sepstr_q(std::string_view input) {
    std::vector<std::string_view> ret;
    while (!input.empty()) {
        // Discard whitespace
        std::size_t non_white_start = input.find_first_not_of(' ');
        if (non_white_start == std::string_view::npos) break;
        input.remove_prefix(non_white_start);

        // Find delim
        std::size_t first_delim = input.find_first_of(" \"\'");
        if (first_delim == std::string_view::npos) break;
        std::size_t size = first_delim;

        // act on delim
        if (input[first_delim] == ' ') {
            ret.push_back(input.substr(0, first_delim - 1));
            input.remove_prefix(first_delim);
        } else {
            // Pushback existing
            if (first_delim != 0) ret.push_back(input.substr(0, size));
            // Find end
            char found_delim = input[first_delim];
            input.remove_prefix(first_delim);
            std::size_t end = input.find_first_of(input);
            if (end == std::string_view::npos) break;
            ret.push_back(input.substr(0, end - 1));
            input.remove_prefix(end);
        }
    }
    if (!input.empty()) ret.push_back(input);
    return ret;
}

namespace fuz {

const char* strstr(const char* main, const char* sub) {
    if constexpr (plat::kLinux)
        return strcasestr(main, sub);
    else {
        std::size_t find =
            fuz::strstr(std::string_view(main), std::string_view(sub));
        return (find == std::string_view::npos) ? nullptr : &main[find];
    }
}
std::size_t strstr(std::string_view main, std::string_view sub) {
    if (sub.size() > main.size()) return std::string_view::npos;
    for (int i = 0; i < main.size() - sub.size(); i++) {
        int ii;
        for (ii = 0; ii < sub.size(); ii++) {
            if (tolower(main[i + ii]) != tolower(sub[ii])) continue;
        }
        if (ii >= sub.size()) return i;
    }
    return std::string_view::npos;
}

int strcmp(const char* str1, const char* str2) {
    if constexpr (plat::kLinux)
        return strcasecmp(str1, str2);
    else {
        // Only linux has strcasecmp, so we need to recreate it for other
        // systems
        size_t size_1 = strlen(str1), size_2 = strlen(str2);
        if (size_1 != size_2) return size_2 - size_1;
        for (size_t i = 0; i < size_1; i++)
            if (str1[i] != str2[i] || tolower(str1[i]) != tolower(str2[i]))
                return str2[i] - str1[i];  // Maintain normal behaviour
        return 0;
    }
}
inline bool strcmp(std::string_view str1, std::string_view str2) {
    if (str1.size() != str2.size()) return false;
    for (size_t i = 0; i < str1.size(); i++)
        if (str1[i] != str2[i] && tolower(str1[i]) != tolower(str2[i]))
            return false;
    return true;
}

}  // namespace fuz
}  // namespace neko
