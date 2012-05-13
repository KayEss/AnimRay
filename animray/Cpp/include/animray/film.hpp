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


#ifndef ANIMRAY_FILM_HPP
#define ANIMRAY_FILM_HPP
#pragma once


#include <animray/extents2d.hpp>
#include <fost/exception/out_of_range.hpp>


namespace animray {


    /// A film represents a raster of pixel data in columns
    template< typename C, typename E = std::size_t >
    class film {
        typedef std::vector< C > column_type;
        typedef std::vector< column_type > columns_type;
        columns_type columns;
        public:
            /// The colour type
            typedef C color_type;
            /// The type of the extents co-ordinates
            typedef E extents_value_type;
            /// The extents type
            typedef extents2d< extents_value_type > extents_type;
            /// The extents size type
            typedef typename extents_type::size_type size_type;

            /// Construct an empty targa of the given size
            film( size_type width, size_type height, const C &colour = C() )
            : columns( width, column_type(height, colour)) {
                if ( width < 1 )
                    throw fostlib::exceptions::out_of_range<E>(
                        1, std::numeric_limits<E>::max(), width);
                if ( height < 1 )
                    throw fostlib::exceptions::out_of_range<E>(
                        1, std::numeric_limits<E>::max(), height);
            }

            /// Construct a film of a given size with a lambda telling us which colors to use
            template< typename F >
            film( size_type width, size_type height, F fn, const C &colour = C() )
            : film(width, height, colour) {
            }

            /// The width of the image
            const size_type width() const {
                return columns.size();
            }
            /// The height of the image
            const size_type height() const {
                return columns[0].size();
            }
            /// Return the extents of the image
            extents_type size() const {
                return extents_type( 0, 0, width() - 1, height() - 1 );
            }

            /// Return a mutable row
            column_type &operator [] ( size_type c ) {
                return columns[c];
            }
            /// Return a non-mutable row
            const column_type &operator [] ( size_type c ) const {
                return columns[c];
            }

            /// Iterate the function across the image rows/columns
            template< typename F >
            void for_each( F fn ) const {
                for ( size_type c = 0; c < width(); ++c ) {
                    const column_type &col = columns[c];
                    for ( size_type r = 0; r < height(); ++r )
                        fn( *this, col[r] );
                }
            }
    };


}


#endif // ANIMRAY_FILM_HPP
