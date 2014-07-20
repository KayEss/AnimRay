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


#ifndef ANIMRAY_LIGHT_HPP
#define ANIMRAY_LIGHT_HPP
#pragma once


namespace animray {


    template< typename L, typename C >
    class light {
        public:
            /// The light geometry
            typedef L geometry_type;
            /// The colour type
            typedef C color_type;

            /// The colour of the light
            fostlib::accessors< color_type > color;

            /// Calculate the illumination given by this light
            template< typename R, typename G >
            color_type operator () (const R &intersection, const G &geometry) const {
                R light(intersection.from(),
                    typename R::end_type(5.0, 5.0, -5.0));
                if ( geometry.occludes(light, 1e-9) ) {
                    return color();
                } else {
                    const double costheta = dot(light.direction(),
                        intersection.direction());
                    return color_type(color() + 205 * costheta);
                }
            }
    };


}


#endif // ANIMRAY_LIGHT_HPP
