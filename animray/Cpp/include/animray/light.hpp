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


    /// Lights allow illumination of the scene
    template< typename L, typename C >
    class light {
        public:
            /// The light geometry
            typedef L geometry_type;
            /// The colour type
            typedef C color_type;

            /// The geometry of the light
            fostlib::accessors< geometry_type > geometry;
            /// Ambient light provided by this light source
            fostlib::accessors< color_type> ambient;
            /// The colour of the light
            fostlib::accessors< color_type > color;

            /// Calculate the illumination given by this light
            template< typename R, typename G >
            color_type operator () (const R &intersection, const G &scene) const {
                R illumination(intersection.from(), geometry());
                if ( scene.occludes(illumination, 1e-9) ) {
                    return ambient();
                } else {
                    const double costheta = dot(illumination.direction(),
                        intersection.direction());
                    return color_type(ambient() + color() * costheta);
                }
            }
    };


}


#endif // ANIMRAY_LIGHT_HPP
