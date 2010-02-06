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


#include <animray/film.hpp>


namespace animray {


    /// An immutable data structure able to hold raster images of varying resolution
    template< typename F >
    class raster {
        /// The cache of the film data
        boost::shared_ptr< F > film_data;
        public:
            /// The film type the raster covers
            typedef F film_type;
            /// The type describing the number of pixels
            typedef typename film_type::extents_value_type resolution_type;
            /// The colour type
            typedef typename film_type::color_type color_type;

            /// We can start by making a raster based on a film
            raster( std::auto_ptr< film_type > film )
            : film_data( film.release() ) {
            }
            /// We can also start by making a blank raster from a resolution
            raster(
                resolution_type w, resolution_type h, const color_type &c = color_type()
            ) : film_data( new film_type( w, h, c ) ) {
            }
            /// Create a new raster by splitting apart a smaller one
            raster(
                const raster &from, resolution_type x_split, resolution_type y_split
            ) : film_data( new film_type(
                from.film().width() * x_split, from.film().height() * y_split
            ) ) {
                for ( resolution_type ox = 0; ox < from.film().width(); ++ox )
                    for ( resolution_type oy = 0; oy < from.film().height(); ++oy )
                        for ( resolution_type cx = 0; cx < x_split; ++cx )
                            for ( resolution_type cy = 0; cy < y_split; ++cy )
                                (*film_data)[ ox * x_split + cx ][ oy * y_split + cy ] =
                                    from.film()[ ox ][ oy ];
            }

            /// Provide access to the actual raster data
            const film_type &film() const {
                return *film_data;
            }
    };


}


#endif // ANIMRAY_RASTER_HPP
