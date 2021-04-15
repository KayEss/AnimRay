/**
    Copyright 2021, [Kirit Saelensminde](https://kirit.com/AnimRay).

    This file is part of AnimRay.

    AnimRay is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    AnimRay is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with AnimRay.  If not, see <http://www.gnu.org/licenses/>.
*/


#pragma once


#include <cinttypes>
#include <filesystem>
#include <map>


namespace animray::cli {


    class arguments {
      public:
        arguments(
                int argc,
                char const *const argv[],
                const char *o,
                std::size_t const w,
                std::size_t const h)
        : output_filename{o}, width{w}, height{h} {
            std::optional<char> option;
            for (std::size_t arg{1}; arg < argc; ++arg) {
                if (argv[arg][0] == '-'
                    and (argv[arg][1] < '0' or argv[arg][1] > '9')) {
                    if (argv[arg][1] != 0) {
                        option = argv[arg][1];
                        switches[*option] = nullptr;
                    } else {
                        throw std::runtime_error{
                                "No option specified after switch character"};
                    }
                } else if (option) {
                    switches[*option] = argv[arg];
                    option = {};
                } else {
                    throw std::runtime_error{"All arguments are switches"};
                }
            }
            if (auto const oi = switches.find('o'); oi != switches.end()) {
                output_filename = oi->second;
            }
            if (auto const wi = switches.find('w'); wi != switches.end()) {
                char *ends = nullptr;
                width = std::strtoimax(wi->second, &ends, 10);
            }
            if (auto const hi = switches.find('h'); hi != switches.end()) {
                char *ends = nullptr;
                height = std::strtoimax(hi->second, &ends, 10);
            }
        }

        /// Common parameters
        std::filesystem::path output_filename;
        std::size_t width, height;

        /// Other switches and parameters
        std::map<char, char const *> switches;

        /// APIs to make things simpler
        int switch_value(char const option, int const v) const {
            if (auto const it = switches.find(option); it != switches.end()) {
                char *ends;
                return std::atoi(it->second);
            } else {
                return v;
            }
        }
        unsigned int
                switch_value(char const option, unsigned int const v) const {
            if (auto const it = switches.find(option); it != switches.end()) {
                char *ends;
                return std::strtoul(it->second, &ends, 10);
            } else {
                return v;
            }
        }
        std::size_t switch_value(char const option, std::size_t const v) const {
            if (auto const it = switches.find(option); it != switches.end()) {
                char *ends;
                return std::strtoimax(it->second, &ends, 10);
            } else {
                return v;
            }
        }

        float switch_value(char const option, float const v) const {
            if (auto const it = switches.find(option); it != switches.end()) {
                char *ends;
                return std::strtof(it->second, &ends);
            } else {
                return v;
            }
        }
        double switch_value(char const option, double const v) const {
            if (auto const it = switches.find(option); it != switches.end()) {
                char *ends;
                return std::strtod(it->second, &ends);
            } else {
                return v;
            }
        }
        long double switch_value(char const option, long double const v) const {
            if (auto const it = switches.find(option); it != switches.end()) {
                char *ends;
                return std::strtold(it->second, &ends);
            } else {
                return v;
            }
        }
    };


}
