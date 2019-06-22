
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

#include <cstdint>

namespace neko {

// TODO, move constants to the lib
class RGBColor {
   public:
    // Constructors
    RGBColor(uint8_t _r, uint8_t _g, uint8_t _b, uint8_t _a = 255)
        : r(_r), g(_g), b(_b), a(_a) {}
    uint8_t r, g, b, a;
    inline bool operator==(const RGBColor& in) const {
        return this->r == in.r && this->g == in.g && this->b == in.b &&
               this->a == in.a;
    }
    // Percentage u want it to be
    inline RGBColor Transparent(int p) {
        return RGBColor(
            this->r, this->g, this->b,
            static_cast<int>(static_cast<float>(this->a) / 100) * p);
    }
    static RGBColor FromHSL(float h, float s, float v);
    static RGBColor ToRGBA8(float r, float g, float b, float a = 1.0f) {
        return RGBColor(static_cast<int>(r * 255), static_cast<int>(g * 255),
                        static_cast<int>(b * 255), static_cast<int>(a * 255));
    };

    // Static Colors
   public:
    static const RGBColor white;
    static const RGBColor black;
    static const RGBColor pink;
    static const RGBColor red;
    static const RGBColor blue;
    static const RGBColor yellow;
    static const RGBColor orange;
    static const RGBColor green;
    static const RGBColor gray;
    static const RGBColor lightgray;
    static const RGBColor darkgray;
    static const RGBColor empty;

    // Returns a rainbow color based on time
    static RGBColor Rainbow();
    static RGBColor Random();
};

}  // namespace neko
