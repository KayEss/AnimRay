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


#include <animray/affine.hpp>
#include <animray/functional/traits.hpp>
#include <animray/ray.hpp>
#include <felspar/test.hpp>


namespace {


    auto const suite = felspar::testsuite(__FILE__);


    static_assert(animray::Regular<animray::ray<int>>);
    static_assert(animray::Regular<animray::ray<int64_t>>);
    static_assert(animray::Regular<animray::ray<float>>);
    static_assert(animray::Regular<animray::ray<double>>);
    static_assert(animray::Regular<animray::ray<long double>>);


    auto const t = suite.test("transformation", [](auto check) {
        animray::ray r(animray::point3d(0, 0, 0), animray::point3d(0, 0, 1));
        auto aff(animray::translate<int>(10, 23, 54));
        animray::ray<int> rf(r * aff.forward());
        check(rf.from) == animray::ray<int>::end_type(10, 23, 54);
        check(rf.direction) == animray::ray<int>::direction_type(0, 0, 1);

        animray::ray<int> rb(r * aff.backward());
        check(rb.from) == animray::ray<int>::end_type(-10, -23, -54);
        check(rb.direction) == animray::ray<int>::direction_type(0, 0, 1);

        check((r * aff.forward() * aff.backward()).from) == r.from;
        check((r * aff.forward() * aff.backward()).direction) == r.direction;
        check((r * aff.backward() * aff.forward()).from) == r.from;
        check((r * aff.backward() * aff.forward()).direction) == r.direction;
    });


    auto const c = suite.test("comparison", [](auto check) {
        check(animray::ray<int>())
                == animray::ray<int>(
                        animray::point3d(0, 0, 0), animray::point3d(0, 0, 1));
        check(animray::ray<int>())
                != animray::ray<int>(
                        animray::point3d(1, 0, 0), animray::point3d(2, 0, 0));
        check(animray::ray(animray::point3d(5, 5, 0), animray::point3d(5, 5, 1)))
                == animray::ray<int>(
                        animray::point3d(5, 5, 0),
                        animray::unit_vector<int>(0, 0, 1));
        check(animray::ray<float>(
                animray::point3d<float>(0, 0, 1),
                animray::point3d<float>(1, 1, 0)))
                == animray::ray<float>(
                        animray::point3d<float>(0, 0, 1),
                        animray::unit_vector<float>(
                                1, 1, -1, 1.7320508075688772));
    });


}
