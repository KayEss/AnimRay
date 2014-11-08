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


#ifndef ANIMRAY_EMISSION_HPP
#define ANIMRAY_EMISSION_HPP
#pragma once


namespace animray {


    /// Non-emissive surface
    template<typename C, typename RI, typename I, typename G>
    struct surface_emission {
        surface_emission() {}
        C operator() (const RI&, const I &, const G &) const {
            return C();
        }
    };


    /// Calls in to the relevant surface emission handler
    template<typename C, typename RI, typename I, typename G>
    C emission(const RI &observer, const I &intersection, const G &scene) {
        return surface_emission<C, RI, I, G>()(observer, intersection, scene);
    }


}


#endif // ANIMRAY_EMISSION_HPP
