/**
    Copyright 2014-2021, [Kirit Saelensminde](https://kirit.com/AnimRay).

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


#ifndef ANIMRAY_SURFACE_MATTE_HPP
#define ANIMRAY_SURFACE_MATTE_HPP
#pragma once


#include <animray/surface.hpp>


namespace animray {


    /// The matte surface intersection type
    template<typename C>
    class matte {
      public:
        /// Default constructor
        matte() = default;
        matte(C c) : attenuation{std::move(c)} {}

        /// The absorption attenuation of the surface
        C attenuation;

        /// Calculate the light/surface interaction
        template<typename RI, typename RL, typename I, typename CI, typename G>
        CI operator()(
                const RI &,
                const RL &light,
                const I &intersection,
                const CI &incident,
                const G &) const {
            using D = typename RI::local_coord_type;
            const D costheta = dot(light.direction, intersection.direction);
            return incident * attenuation * costheta;
        }

        /// This material is non-emissive
        template<typename CI, typename RI, typename I, typename G>
        CI operator()(const CI &, const RI &, const I &, const G &) const {
            return CI();
        }
    };


}


#endif // ANIMRAY_SURFACE_MATTE_HPP
