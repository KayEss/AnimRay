/*
    Copyright 1995-2010, Kirit Saelensminde.
    http://www.kirit.com/AnimRay

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

#include <boost/lambda/bind.hpp>
#include <boost/filesystem/fstream.hpp>


namespace animray {


    namespace detail {
        /// Used to implement the Targa file saving. Do not use directly
        template< typename C, typename E >
        struct targa_saver;
    }


    /// Save a film as a Targa file
    template< typename C, typename E >
    void targa(
        const boost::filesystem::wpath &filename,
        const film< C, E > &f
    ) {
        detail::targa_saver< C, E >()( filename, f );
    }


    namespace detail {
        /// Save 8 bit films as Targa file
        template< typename E >
        struct targa_saver< uint8_t, E > {
            void operator () (
                const boost::filesystem::wpath &filename,
                const film< uint8_t, E > &image
            ) const {
                boost::filesystem::ofstream file( filename, std::ios::binary );
                // Header
                file.put(0); // 0 identsize
                file.put(0); // Has no colour map
                file.put(3); // Uncompressed grayscale image (as this is uint8_t)
                file.put(0); file.put(0); // Colour map offset
                file.put(0); file.put(0); // Colour map indexes
                file.put(0); // Colour map bits per pixel
                file.put(0); file.put(0); // X origin
                file.put(0); file.put(0); // Y origin
                uint16_t w( fostlib::coerce< uint16_t >(image.width()) );
                uint16_t h( fostlib::coerce< uint16_t >(image.height()) );
                file.write(reinterpret_cast< const char * >(&w), 2);
                file.write(reinterpret_cast< const char * >(&h), 2);
                file.put(8); // 8 bit pixels
                file.put(0); // Image data starts bottom left with zero alpha channel
                // Image data
                image.for_each(boost::lambda::bind(
                    &boost::filesystem::ofstream::put, boost::ref(file), boost::lambda::_2
                ));
                // Footer (for Targa 2)
                file.put(0); file.put(0); // Targa 2 extension data size
                file << "TRUEVISION-XFILE.";
                file.put(0);
            }
        };
    }


}


#endif // ANIMRAY_TARGA_HPP
