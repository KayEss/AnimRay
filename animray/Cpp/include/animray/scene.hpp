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


#ifndef ANIMRAY_SCENE_HPP
#define ANIMRAY_SCENE_HPP
#pragma once


namespace animray {


    /// A scene featuring a light and a model
    template< typename G, typename L, typename C >
    class scene {
    public:
        /// The geometry type
        typedef G geometry_type;
        /// The light type
        typedef L light_type;
        /// The colour type
        typedef C color_type;
        /// The type of the rays used
        typedef typename geometry_type::ray_type ray_type;

        /// Construct an empty scene
        scene() {}

        /// Store the geometry
        fostlib::accessors<geometry_type, fostlib::lvalue> geometry;
        /// Store the light
        fostlib::accessors<light_type, fostlib::lvalue> light;
        /// Background colour
        fostlib::accessors<color_type> background;

        /// Given a position on the camera film, calculate the colour it should be
        template< typename M, typename S >
        color_type operator() (const M &camera, S x, S y) const {
            fostlib::nullable<ray_type>
                intersection(geometry().intersection(camera(x, y)));
            if ( !intersection.isnull() ) {
                return color_type(light()(intersection.value(), geometry()));
            } else {
                return background();
            }
        }
    };


}


#endif // ANIMRAY_SCENE_HPP
