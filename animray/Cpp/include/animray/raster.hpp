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


#ifndef ANIMRAY_RASTER_HPP
#define ANIMRAY_RASTER_HPP
#pragma once


#include <fost/core>


namespace animray {


    /// A permanent data structure able to hold raster images of varying resolution
    template<
        typename C, typename E,
        typename R = typename E::size_type
    > class raster {
        typedef C pixel_type;
        typedef std::vector< pixel_type > pixels_type;
        pixels_type pixel_data;
        public:
            /// The type of colour data held within the raster
            typedef C color_type;
            /// The way that extents are held within the raster
            typedef E extents_type;
            /// The type of the resolution
            typedef R resolution_type;

            /// Constructs a raster covering a particular co-ordinate range
            raster( const extents_type &e, resolution_type pixel_area )
            : pixel_data( std::size_t( e.area() / pixel_area ) ),
                extents( e ), pixel_area( pixel_area )
            {
            }
            /// Constructs a raster covering a particular co-ordinate range
            raster(
                const extents_type &e, std::size_t x_pixels, std::size_t y_pixels
            ) : pixel_data( x_pixels * y_pixels ),
                extents( e ), pixel_area( e.area() / ( x_pixels * y_pixels ) )
            {
            }

            /// Stores the extents of the raster
            fostlib::accessors< extents_type > extents;
            /// Stores the width in pixels
            fostlib::accessors< std::size_t > width;
            /// Stores the height in pixels
            fostlib::accessors< std::size_t > height;
            /// Stores the pixel area of the raster
            fostlib::accessors< resolution_type > pixel_area;
            /// The number of pixels
            std::size_t pixels() const {
                return pixel_data.size();
            }
    };


}


#endif // ANIMRAY_RASTER_HPP
