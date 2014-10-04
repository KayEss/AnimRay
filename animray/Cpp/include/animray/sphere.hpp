/*
    Copyright 1995-2014, Kirit Saelensminde.
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
    template< typename I, typename D = typename I::local_coord_type >
    class sphere {
    public:
        /// The type of the local coordinates used
        typedef D local_coord_type;
        /// Type of intersection to return when the sphere is struck
        typedef I intersection_type;

        /// Check for equality
        bool operator == ( const sphere &r ) const {
            return true;
        }
        /// Check for inequality
        bool operator != ( const sphere &r ) const {
            return false;
        }

        /// Returns a ray giving the intersection point and surface normal or
        /// null if no intersection occurs
        template<typename R, typename E>
        fostlib::nullable< intersection_type > intersects(
            const R &by, const E epsilon
        ) const {
            const D b = D(2) * dot(by.from(), by.direction());
            const D c = by.from().dot() - D(1);
            const D discriminant = b * b - D(4) * c;
            if ( discriminant < D(0) ) return fostlib::null;
            const D disc_root = std::sqrt( discriminant );
            const D t0 = (-b - disc_root) / D(2);
            const D t1 = (-b + disc_root) / D(2);
            const D t = t0 < D(0) ? t1 : t0;
            if ( t < epsilon ) return fostlib::null;
            typedef typename ray<D>::end_type end_type;
            typedef typename ray<D>::direction_type direction_type;
            direction_type normal(by.from() + by.direction() * t);
            return intersection_type(end_type(normal), normal);
        }

        /// Returns true if the ray hits the sphere
        template< typename R >
        bool occludes(const R &by, const D epsilon) const {
            // http://www.siggraph.org/education/materials/HyperGraph/
            const D b = D(2) * dot(by.from(), by.direction());
            const D c = by.from().dot() - D(1);
            const D discriminant = b * b - D(4) * c;
            if ( discriminant < D(0) ) return false;
            const D disc_root = std::sqrt( discriminant );
            if ( -b - disc_root >= epsilon ) return true;
            if ( -b + disc_root >= epsilon ) return true;
            return false;
        }
    };


}


#endif // ANIMRAY_SPHERE_HPP
