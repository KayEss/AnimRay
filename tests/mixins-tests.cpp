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


#include <animray/ray.hpp>
#include <animray/mixins/depth-count.hpp>
#include <animray/mixins/time.hpp>
#include <felspar/test.hpp>


namespace {
    auto const suite = felspar::testsuite(__FILE__);


    auto const dc = suite.test("depth_count", [](auto check) {
        class base {};
        static_assert(
                not std::is_same<
                        typename animray::with_depth_count<base>::type,
                        base>::value,
                "We should get different types when we add in the depth count");
        using with_count = typename animray::with_depth_count<base>::type;
        with_count counted{};
        check(counted.depth_count) == 0u;
        counted.add_count(base());
        check(counted.depth_count) == 1u;
        static_assert(
                std::is_same<
                        with_count,
                        typename animray::with_depth_count<with_count>::type>::
                        value,
                "The depth count is already added, so expect the same type");
        typename animray::with_depth_count<with_count>::type two(counted);
        two.add_count(counted);
        check(two.depth_count) == 2u;
    });


    auto const dcm = suite.test("depth_count_multiply", [](auto check) {
        animray::with_depth_count<animray::ray<int>>::type r(
                animray::ray<int>::end_type(0, 0, 0),
                animray::ray<int>::end_type(0, 0, 1));
        r.add_count(animray::ray<int>());
        check(r.depth_count) == 1u;
        animray::with_depth_count<animray::ray<int>>::type m(
                r * animray::matrix<int>());
        check(m.depth_count) == 1u;
    });


    auto const time = suite.test("time", [](auto check) {
        class base {};
        static_assert(
                not std::is_same<
                        typename animray::with_time<base>::type, base>::value,
                "We should get different types when we add in the time "
                "information");
        using with_time = typename animray::with_time<base>::type;
        with_time at_time;
        check(at_time.time) == std::chrono::system_clock::time_point{};
        static_assert(
                std::is_same<
                        with_time,
                        typename animray::with_time<with_time>::type>::value,
                "The time is already added, so expect the same type");
    });


}
