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
#include <animray/maths/angles.hpp>
#include <animray/ray.hpp>
#include <animray/test.hpp>
#include <felspar/test.hpp>


namespace {


    auto const suite = felspar::testsuite(__FILE__);


    static_assert(animray::Regular<animray::matrix<int>>);
    static_assert(animray::Regular<animray::matrix<float>>);


    auto const mm = suite.test("matrix multiply", [](auto check) {
        animray::matrix<int> id;

        auto aff(animray::translate<int>(10, 23, 54));
        check(aff.forward() * aff.backward()) == id;
        check(id * aff.forward()) == aff.forward();
        check(id * aff.backward()) == aff.backward();

        animray::point3d<int> pf(aff.forward() * animray::point3d<int>());
        check(pf.x()) == 10;
        check(pf.y()) == 23;
        check(pf.z()) == 54;

        animray::point3d<int> ps(aff.backward() * animray::point3d<int>());
        check(ps.x()) == -10;
        check(ps.y()) == -23;
        check(ps.z()) == -54;
    });


    auto const rrf = suite.test("rotate ray forward", [](auto check) {
        animray::ray<double> z(
                animray::point3d<double>(0, 0, 0),
                animray::unit_vector<double>(0, 0, 1));
        check(z.direction) == (z.ends() - z.from).unit();

        animray::matrix<double> rx45(animray::rotate_x<double>(-45_deg).first);
        animray::ray<double> r45(z * rx45);
        check(r45.direction) == (r45.ends() - r45.from).unit();

        animray::ray<double> r90(r45 * rx45);
        check(r90.direction) == (r90.ends() - r90.from).unit();
        animray::check_close(check, r90.direction.x(), 0, 1e-10);
        animray::check_close(check, r90.direction.y(), 1, 1e-10);
        check(r90.direction.z()) < 1e-10;
    });


    auto const rrb = suite.test("rotate ray backward", [](auto check) {
        animray::ray<double> z(
                animray::point3d<double>(0, 0, 0),
                animray::unit_vector<double>(0, 0, 1));
        check(z.direction) == (z.ends() - z.from).unit();

        animray::matrix<double> rx45(animray::rotate_x<double>(45_deg).second);
        animray::ray<double> r45(z * rx45);
        check(r45.direction) == (r45.ends() - r45.from).unit();

        animray::ray<double> r90(r45 * rx45);
        check(r90.direction) == (r90.ends() - r90.from).unit();
        animray::check_close(check, r90.direction.x(), 0, 1e-10);
        animray::check_close(check, r90.direction.y(), 1, 1e-10);
        check(r90.direction.z()) < 1e-10;
    });


    auto const rs = suite.test("ray scale", [](auto check) {
        animray::ray<double> all(
                animray::point3d<double>(0, 0, 0),
                animray::point3d<double>(1, 1, 1));
        check(all.direction) == (all.ends() - all.from).unit();

        animray::matrix<double> halve(
                animray::scale<double>(0.5, 0.5, 0.5).first);
        animray::ray<double> havled(all * halve);
        check(all) == havled;
    });


}
