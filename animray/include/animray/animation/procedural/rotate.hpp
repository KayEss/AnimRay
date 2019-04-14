/*
    Copyright 2014, Kirit Saelensminde.
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


#ifndef ANIMRAY_ANIMATION_PROCEDURAL_ROTATE_HPP
#define ANIMRAY_ANIMATION_PROCEDURAL_ROTATE_HPP
#pragma once


#include <animray/animation/animate.hpp>
#include <animray/interpolation/linear.hpp>


namespace animray {


    namespace animation {


        /// Return a point that rotates about a centre position in the x/y plane
        template<typename P>
        class rotate_xy {
          public:
            /// The type of point that is to be returned
            typedef P point_type;
            /// The value type from the point type
            typedef typename point_type::value_type value_type;
            /// The type of the radius
            typedef typename point_type::value_type radius_type;
            /// The type of the speed
            typedef typename point_type::value_type speed_type;
            /// The type of the phase offset
            typedef typename point_type::value_type phase_type;

            /// The centre to rotate about
            fostlib::accessors<point_type> centre;
            /// The radius
            fostlib::accessors<radius_type> radius;
            /// Rotations per unit time in radians/time unit
            fostlib::accessors<speed_type> speed;
            /// Phase offset
            fostlib::accessors<phase_type> phase;

            /// Allow default construction
            rotate_xy() : radius(1), speed(1) {}

            /// Construct the rotation parameters
            rotate_xy(
                    const point_type &c,
                    const radius_type r,
                    const speed_type s,
                    const phase_type p)
            : centre(c), radius(r), speed(s), phase(p) {}

            /// Calculate the position for the requested frame
            template<typename T>
            point_type operator()(const T t) const {
                return point_type(
                        centre().x()
                                + radius() * std::cos(t * speed() + phase()),
                        centre().y()
                                + radius() * std::sin(t * speed() + phase()),
                        centre().z());
            }
        };


    }


}


#endif // ANIMRAY_ANIMATION_PROCEDURAL_ROTATE_HPP
