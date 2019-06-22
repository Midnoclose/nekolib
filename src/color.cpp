
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

#include <chrono>
#include <cmath>

#include <neko/color.hpp>

namespace neko {

// Color wheel func, credits goto cathook for this
RGBColor RGBColor::FromHSL(float h, float s, float v) {
    if (s <= 0.0f) return RGBColor::ToRGBA8(v, v, v);
    if (h >= 360.0f) h = 0.0f;
    h /= 60.0f;
    long i = long(h);
    float ff = h - i;
    float p = v * (1.0f - s);
    float q = v * (1.0f - (s * ff));
    float t = v * (1.0f - (s * (1.0f - ff)));

    switch (i) {
        case 0:
            return RGBColor::ToRGBA8(v, t, p);
        case 1:
            return RGBColor::ToRGBA8(q, v, p);
        case 2:
            return RGBColor::ToRGBA8(p, v, t);
        case 3:
            return RGBColor::ToRGBA8(p, q, v);
        case 4:
            return RGBColor::ToRGBA8(t, p, v);
    }
    return RGBColor::ToRGBA8(v, p, q);
}

static auto start_time = std::chrono::steady_clock::now();
RGBColor RGBColor::Rainbow() {
    std::chrono::duration<float, std::deca> curtime =
        std::chrono::steady_clock::now() - start_time;
    return RGBColor::FromHSL(fabs(sin(curtime.count())) * 360.0f, 0.85f, 0.9f);
}

const RGBColor RGBColor::white(255, 255, 255);
const RGBColor RGBColor::black(0, 0, 0);
const RGBColor RGBColor::pink(255, 105, 180);
const RGBColor RGBColor::red(237, 42, 42);
const RGBColor RGBColor::blue(28, 108, 237);
const RGBColor RGBColor::yellow(255, 255, 0);
const RGBColor RGBColor::orange(255, 120, 0);
const RGBColor RGBColor::green(0, 255, 0);
const RGBColor RGBColor::gray(100, 100, 100);
const RGBColor RGBColor::lightgray(180, 180, 180);
const RGBColor RGBColor::darkgray(50, 50, 50);
const RGBColor RGBColor::empty(0, 0, 0, 0);

}  // namespace neko
