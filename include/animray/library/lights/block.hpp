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


#pragma once


#include <animray/color/luma.hpp>
#include <animray/color/rgb.hpp>
#include <animray/light/ambient.hpp>
#include <animray/light/collection.hpp>
#include <animray/light/point.hpp>


namespace animray::library::lights {


    template<typename World>
    using bulb = animray::light<animray::point3d<World>, animray::rgb<float>>;

    /**
     * Block of lights of three colours with some ambient.
     */
    template<typename World>
    auto constexpr narrow_spots = animray::light{animray::make_array(
            bulb<World>{{-3.0, 5.0, -5.0}, {0x40, 0xa0, 0x40}},
            bulb<World>{{-5.0, -3.0, -5.0}, {0xa0, 0x40, 0x40}},
            bulb<World>{{3.0, -3.0, -5.0}, {0x40, 0x40, 0xa0}})};
    template<typename World>
    auto constexpr narrow_block = animray::light{
            narrow_spots<World>, animray::light{animray::luma{50.0f}}};

    template<typename World>
    auto constexpr wide_spots = animray::light{animray::make_array(
            bulb<World>{{-5.0, 5.0, -5.0}, {0x40, 0xa0, 0x40}},
            bulb<World>{{-5.0, -5.0, -5.0}, {0xa0, 0x40, 0x40}},
            bulb<World>{{5.0, -5.0, -5.0}, {0x40, 0x40, 0xa0}})};
    template<typename World>
    auto constexpr wide_block = animray::light{
            wide_spots<World>, animray::light{animray::luma{50.0f}}};


}
