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


#ifndef ANIMRAY_POINT2D_HPP
#define ANIMRAY_POINT2D_HPP
#pragma once


#include <fost/core>


namespace animray {


    /// A 2d point
    template< typename C >
    class point2d {
        public:
            /// The type of the location values
            typedef C value_type;

            /// The x location
            fostlib::accessors< C > x;
            /// The y location
            fostlib::accessors< C > y;

            /// Construct an empty point
            point2d() {
            }
            /// Construct a point from a pair of co-ordinates
            point2d( const C &x, const C &y )
            : x(x), y(y) {
            }

            /// Check for equality
            bool operator == ( const point2d &r ) const {
                return x() == r.x() && y() == r.y();
            }
            /// Check for inequality
            bool operator != ( const point2d &r ) const {
                return x() != r.x() || y() != r.y();
            }
    };


}


/// Allow two points to be added together
template< typename C >
animray::point2d< C > operator + (
    const animray::point2d< C > &a, const animray::point2d< C > &b
) {
    return animray::point2d< C >( a.x() + b.x(), a.y() + b.y() );
}


namespace fostlib {
    template< typename C >
    struct coercer< fostlib::json, animray::point2d< C > > {
        fostlib::json coerce( const animray::point2d< C > &p ) {
            fostlib::json j; fostlib::jcursor r;
            r.push_back(j, fostlib::coerce< fostlib::json >( p.x() ));
            r.push_back(j, fostlib::coerce< fostlib::json >( p.y() ));
            return j;
        }
    };
    template< typename C >
    struct coercer< animray::point2d< C >, fostlib::json > {
        animray::point2d< C > coerce( const fostlib::json &j ) {
            return animray::point2d< C >(
                fostlib::coerce< typename animray::point2d< C >::value_type >( j[0] ),
                fostlib::coerce< typename animray::point2d< C >::value_type >( j[1] )
            );
        }
    };
}


namespace std {
    /// Allow 2d points to be displayed on a stream
    template< typename C >
    inline fostlib::ostream &operator << (
        fostlib::ostream &o, const animray::point2d< C > &p
    ) {
        return o << "(" << p.x() << ", " << p.y() << ")";
    }
}


#endif // ANIMRAY_POINT2D_HPP
