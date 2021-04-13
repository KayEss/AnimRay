/**
    Copyright 2010-2021, [Kirit Saelensminde](https://kirit.com/AnimRay).

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


#include <animray/functional/traits.hpp>
#include <animray/unit-vector.hpp>
#include <felspar/test.hpp>


namespace {


    auto const suite = felspar::testsuite(__FILE__);


    static_assert(animray::Regular<animray::unit_vector<int>>);
    static_assert(animray::Regular<animray::unit_vector<int64_t>>);
    static_assert(animray::Regular<animray::unit_vector<float>>);
    static_assert(animray::Regular<animray::unit_vector<double>>);
    static_assert(animray::Regular<animray::unit_vector<long double>>);


    auto const c = suite.test("comparison", [](auto check) {
        check(animray::unit_vector<int>())
                == animray::unit_vector<int>(0, 0, 1);
    });


}
