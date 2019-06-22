
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

#if defined(_MSC_VER)
#include <experimental/filesystem>
namespace fs = std::experimental::filesystem;
#else
#include <filesystem>
namespace fs = std::filesystem;
#endif

#include <neko/thread.hpp>

namespace neko::io {

// Very Primitive IO functions
std::vector<std::string> ReadFile(const fs::path& path);
void WriteFile(const fs::path& path, const std::vector<std::string>& to_write);

// An asynchronous file trail
class FileTrail {
   public:
    FileTrail(const fs::path& _path,
              std::function<void(const std::string&)> _callback)
        : path(_path), callback(_callback) {
        this->thread_loop.Set([this]() { this->ThreadTick(); });
    }

   private:
    const fs::path path;
    const std::function<void(const std::string&)> callback;
    std::streampos cur_pos;
    thread::Loop thread_loop;
    void ThreadTick();
};

class PackedFile {
   public:
    // To construct this, include a file with a char array of your file in
    // binary and input it here
    PackedFile(const unsigned char* _file_start, size_t _file_size);
    ~PackedFile();

    const fs::path& GetPath();
    std::pair<const unsigned char*, size_t> GetData() {
        return {file_start, file_size};
    }

   private:
    fs::path path;                    // path to the unpackedfile
    const unsigned char* file_start;  // Starting point for file in binary
    const size_t file_size;
};

}  // namespace neko::io
