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


namespace animray {


    /// A film represents a raster of pixel data
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
            : columns( width, column_type(height, colour)),
            width( width ), height( height ) {
            }

            /// The width of the image
            fostlib::accessors< const size_type > width;
            /// The height of the image
            fostlib::accessors< const size_type > height;
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

            /// Iterate the given function across the image and allow it to mutate the image
            template< typename F >
            void transform( const F &fn ) {
                transform( fn, size() );
            }
            /// Iterate the given function across the image and allow it to mutate the image
            template< typename F >
            void transform( const F &fn, const extents_type &area ) {
                for ( size_type c = area.lower_left().x(); c <= area.top_right().x(); ++c ) {
                    column_type &col = columns[c];
                    for ( size_type r = area.lower_left().y(); r <= area.top_right().y(); ++r )
                        col[r] = fn( *this,
                            typename extents_type::corner_type(c, r),
                            col[r]
                        );
                }
            }
            /// Iterate the function across the image rows/columns
            template< typename F >
            void for_each( const F &fn ) const {
                for ( size_type c = 0; c < width(); ++c ) {
                    const column_type &col = columns[c];
                    for ( size_type r = 0; r < height(); ++r )
                        fn( *this, col[r] );
                }
            }
    };


}


#endif // ANIMRAY_FILM_HPP
