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


#ifndef ANIMRAY_SPHERE_HPP
#define ANIMRAY_SPHERE_HPP
#pragma once


#include <animray/ray.hpp>


namespace animray {


    /// A simple 3D unit sphere primitive at the origin.
    template< typename D >
    class sphere {
        public:
            /// Check for equality
            bool operator == ( const sphere &r ) const {
                return true;
            }
            /// Check for inequality
            bool operator != ( const sphere &r ) const {
                return false;
            }

            /// Returns true if the ray hits the sphere from the outside
            bool occludes( const ray< D > &by ) const {
                // http://www.siggraph.org/education/materials/HyperGraph/
                const D x0 = by.from().x(), y0 = by.from().y(), z0 = by.from().z();
                const D b = D(2) * (
                    x0 * by.direction().x() +
                        y0 * by.direction().y() +
                            z0 * by.direction().z());
                const D c = x0 * x0 + y0 * y0 + z0 * z0 - D(1);
                const D discriminant = b * b - D(4) * c;
                if ( discriminant < D(0) ) return false;
                const D disc_root = std::sqrt( discriminant );
                if ( -b - disc_root >= 0 ) return true;
                if ( -b + disc_root >= 0 ) return true;
                return false;
            }
    };


}


#endif // ANIMRAY_SPHERE_HPP
