
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
#include <fstream>

#include <neko/filesystem.hpp>

namespace neko::io {

std::vector<std::string> ReadFile(const fs::path& path) {
    // Open the file
    std::ifstream read_stream(path);
    if (!read_stream)
        throw std::runtime_error(
            "Filesystem error: Couldnt open requested file for reading");

    // Recurse through the lines of the file
    std::vector<std::string> ret;
    while (!read_stream.eof()) {
        // Get our line and push to ret
        std::string buffer;
        std::getline(read_stream, buffer);
        ret.push_back(buffer);
    }
    return ret;
}

void WriteFile(const fs::path& path, const std::vector<std::string>& to_write) {
    // Create the file stream
    std::fstream write_stream(path, std::ios::out | std::ios_base::trunc);
    if (!write_stream)
        throw std::runtime_error(
            "Filesystem error: Couldnt open requested file for writing");

    // We dont use an iterator so we can tell where we are in the loop
    auto no_newline = to_write.end() - 1;
    for (auto i = to_write.begin(); i != to_write.end(); i++) {
        write_stream.write(i->c_str(), i->size());
        if (i != no_newline) write_stream << '\n';
    }
}

void FileTrail::ThreadTick() {
    std::ifstream read_stream(path);
    if (!read_stream)
        throw std::runtime_error(
            "Couldnt open file for trailing.");  // This may kill a process

    read_stream.seekg(cur_pos);
    read_stream.clear();

    for (;;) {
        std::string buffer;
        std::getline(read_stream, buffer);
        if (read_stream.eof()) break;

        this->cur_pos = read_stream.tellg();
        this->callback(buffer);
    }

    std::this_thread::sleep_for(std::chrono::seconds(1));
}

PackedFile::PackedFile(const unsigned char* _file_start, size_t _file_size)
    : file_start(_file_start), file_size(_file_size) {
    // Set tmp path
    while (path.empty()) {
        std::string rand_str = "file-a";
        while (rand_str.size() < 15) rand_str += std::to_string(rand() % 15);
        rand_str = rand_str.substr(0, 15);  // Clamp it

        fs::path tmp_path = fs::temp_directory_path() / rand_str;
        if (!std::ifstream(tmp_path)) this->path = tmp_path;
    }

    // Write everything to it
    std::basic_fstream<unsigned char> write_stream(
        this->path, std::ios::out | std::ios_base::trunc);
    if (!write_stream)
        throw std::runtime_error(
            "Filesystem error: couldnt extract packed file");

    write_stream.write(this->file_start, this->file_size);
}

PackedFile::~PackedFile() { fs::remove(this->path); }

}  // namespace neko::io
