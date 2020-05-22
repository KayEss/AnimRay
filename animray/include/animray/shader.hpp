/**
    Copyright 2014-2020, [Kirit Saelensminde](https://kirit.com/AnimRay).

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


    /// Basic Lambertian full-white surface
    template<typename C, typename I, typename RI, typename RL, typename G>
    struct surface_interaction {
        surface_interaction() {}
        C operator()(
                const RI &,
                const RL &light,
                const I &intersection,
                const C &incident,
                const G &) const {
            const typename RI::local_coord_type costheta =
                    dot(light.direction, intersection.direction);
            return incident * costheta;
        }
    };


    /// Calls into the relevant surface partial specialisation
    template<typename RI, typename RL, typename C, typename I, typename G>
    C shader(
            const RI &observer,
            const RL &light,
            const I &intersection,
            const C &incident,
            const G &geometry) {
        const surface_interaction<C, I, RI, RL, G> calculation;
        return calculation(observer, light, intersection, incident, geometry);
    }


}


#endif // ANIMRAY_SHADER_HPP
