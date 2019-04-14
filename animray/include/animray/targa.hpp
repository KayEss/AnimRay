/**
    Copyright 1995-2019, [Kirit Saelensminde](https://kirit.com/AnimRay)

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


#ifndef ANIMRAY_TARGA_HPP
#define ANIMRAY_TARGA_HPP
#pragma once


#include <animray/film.hpp>
#include <animray/color/rgb.hpp>


namespace animray {


    namespace detail {
        /// Used to implement the Targa file saving. Do not use directly
        template<typename C, typename E>
        struct targa_saver;
    }


    /// Save a film as a Targa file
    template<typename C, typename E>
    void targa(const fostlib::fs::path &filename, const film<C, E> &image) {
        detail::targa_saver<C, E> saver;
        fostlib::fs::ofstream file(filename, std::ios::binary);
        // Header
        file.put(0); // 0 identsize
        file.put(0); // Has no colour map
        file.put(saver.type);
        file.put(0);
        file.put(0); // Colour map offset
        file.put(0);
        file.put(0); // Colour map indexes
        file.put(0); // Colour map bits per pixel
        file.put(0);
        file.put(0); // X origin
        file.put(0);
        file.put(0); // Y origin
        uint16_t w(fostlib::coerce<uint16_t>(image.width()));
        uint16_t h(fostlib::coerce<uint16_t>(image.height()));
        file.write(reinterpret_cast<const char *>(&w), 2);
        file.write(reinterpret_cast<const char *>(&h), 2);
        file.put(saver.bits); // n bit pixels
        file.put(0x20); // Image data starts top left with zero alpha channel
        // Image data
        saver(file, image);
        // Footer (for Targa 2)
        file.put(0);
        file.put(0); // Targa 2 extension data size
        file << "TRUEVISION-XFILE.";
        file.put(0);
    }


    namespace detail {
        /// Save 8 bit films as Targa file
        template<typename E>
        struct targa_saver<uint8_t, E> {
            const static char type = 3; // Uncompressed grayscale image
            const static uint8_t bits = 8;
            void operator()(
                    std::ostream &file, const film<uint8_t, E> &image) const {
                typedef typename film<uint8_t, E>::size_type size_type;
                for (size_type r = 0; r < image.height(); ++r)
                    for (size_type c = 0; c < image.width(); ++c)
                        file.put(image[c][r]);
            }
        };

        /// Save 24 bit RGB films as Targa file
        template<typename E>
        struct targa_saver<rgb<uint8_t>, E> {
            const static char type = 2; // Uncompressed RGB image
            const static uint8_t bits = 24;
            void operator()(
                    std::ostream &file, const film<rgb<uint8_t>, E> &image) {
                typedef typename film<rgb<uint8_t>, E>::size_type size_type;
                for (size_type r = 0; r < image.height(); ++r)
                    for (size_type c = 0; c < image.width(); ++c) {
                        rgb<uint8_t> col(image[c][r]);
                        file.put(col.blue());
                        file.put(col.green());
                        file.put(col.red());
                    }
            }
        };
    }


}


#endif // ANIMRAY_TARGA_HPP
