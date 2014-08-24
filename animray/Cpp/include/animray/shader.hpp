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


#ifndef ANIMRAY_SHADER_HPP
#define ANIMRAY_SHADER_HPP
#pragma once


namespace animray {


    template<typename C, typename I, typename R, typename G>
    struct surface_interaction {
        surface_interaction() {}
        C operator() (
            const R &light, const I &intersection, const C &incident, const G &
        ) const {
            const typename R::local_coord_type costheta =
                    dot(light.direction(), intersection.direction());
            return incident * costheta;
        }
    };


    /// Basic Lambertian full-white surface
    template<typename C, typename I, typename R, typename G>
    C shader(
        const R &light, const I &intersection, const C &incident,
        const G &geometry
    ) {
        const surface_interaction<C, I, R, G> calculation;
        return calculation(light, intersection, incident, geometry);
    }


}


#endif // ANIMRAY_SHADER_HPP
