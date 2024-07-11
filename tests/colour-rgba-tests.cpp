/**
    Copyright 2010-2024, [Kirit Saelensminde](https://kirit.com/AnimRay).

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


#include <animray/color/rgba.hpp>
#include <animray/color/concept.hpp>
#include <felspar/test.hpp>

#include <cstdint>


namespace {
    auto const suite = felspar::testsuite("rgba");
}


static_assert(animray::PixelValue<animray::rgba<std::uint8_t>>);
