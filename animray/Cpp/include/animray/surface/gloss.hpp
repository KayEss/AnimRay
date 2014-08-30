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


#ifndef ANIMRAY_SURFACE_GLOSS_HPP
#define ANIMRAY_SURFACE_GLOSS_HPP
#pragma once


#include <animray/surface.hpp>


namespace animray {


    /// The gloss surface intersection type
    template<typename W>
    class gloss {
    public:
        /// Default constructor
        gloss() {}

        /// The width of the specular highlight
        typedef W parameters;

        /// Calculate the light/surface interaction
        template< typename R, typename I, typename CI, typename G >
        CI operator () (
            const W &width, const R &light, const I &intersection,
            const CI &incident, const G &
        ) const {
            return incident;
        }
    };


}


#endif // ANIMRAY_SURFACE_GLOSS_HPP
