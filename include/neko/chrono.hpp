
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

#include <chrono>

namespace neko {

// My version of cathooks Timer class
class Timer {
   private:
    // We use steady as it is constant and cant be changed
    std::chrono::time_point<std::chrono::steady_clock> last_time;

   public:
    inline void Reset() { last_time = std::chrono::steady_clock::now(); }
    inline bool CheckTime(std::chrono::steady_clock::duration dur) const {
        return std::chrono::steady_clock::now() - last_time > dur;
    }
    inline bool ResetAfter(std::chrono::steady_clock::duration dur) {
        auto ret = CheckTime(dur);
        if (ret) Reset();
        return ret;
    }
};

}  // namespace neko
