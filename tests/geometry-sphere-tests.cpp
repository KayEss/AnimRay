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
#include <animray/geometry/quadrics/sphere-unit-origin.hpp>
#include <animray/ray.hpp>
#include <felspar/test.hpp>


namespace {


    auto const suite = felspar::testsuite(__FILE__);


    static_assert(
            animray::Regular<animray::unit_sphere_at_origin<animray::ray<int>>>);
    static_assert(animray::Regular<
                  animray::unit_sphere_at_origin<animray::ray<int64_t>>>);
    static_assert(animray::Regular<
                  animray::unit_sphere_at_origin<animray::ray<float>>>);
    static_assert(animray::Regular<
                  animray::unit_sphere_at_origin<animray::ray<double>>>);
    static_assert(animray::Regular<
                  animray::unit_sphere_at_origin<animray::ray<long double>>>);


    template<typename D, typename C>
    void sphere_occlude(C check) {
        using end_type = typename animray::ray<D>::end_type;
        using ray = animray::ray<D>;
        animray::unit_sphere_at_origin<animray::ray<D>> s;
        check(s.occludes(ray(end_type(0, 0, 10), end_type()), 0)).is_truthy();
        check(s.occludes(ray(end_type(), end_type(0, 0, 10)), 0)).is_truthy();
        check(!s.occludes(ray(end_type(0, 0, 5), end_type(0, 0, 10)), 0))
                .is_truthy();
        check(s.occludes(ray(end_type(0, 0, 10), end_type(0, 0, 5)), 0))
                .is_truthy();
    }
    auto const occ = suite.test("occlusion", [](auto check) {
        sphere_occlude<int>(check);
        sphere_occlude<int64_t>(check);
        sphere_occlude<float>(check);
        sphere_occlude<double>(check);
        sphere_occlude<long double>(check);
    });


}
