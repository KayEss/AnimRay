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


#include <animray/maths/primes.hpp>
#include <felspar/test.hpp>


namespace {


    auto const suite = felspar::testsuite(__FILE__);


    auto const ip = suite.test("is prime", [](auto check) {
        check(animray::is_prime(2)).is_truthy();
        check(animray::is_prime(3)).is_truthy();
        check(animray::is_prime(4)).is_falsey();
        check(animray::is_prime(5)).is_truthy();
        check(animray::is_prime(6)).is_falsey();
        check(animray::is_prime(7)).is_truthy();
        check(animray::is_prime(8)).is_falsey();
        check(animray::is_prime(9)).is_falsey();
        check(animray::is_prime(10)).is_falsey();
        check(animray::is_prime(11)).is_truthy();
        check(animray::is_prime(12)).is_falsey();
        check(animray::is_prime(13)).is_truthy();
        check(animray::is_prime(14)).is_falsey();
        check(animray::is_prime(15)).is_falsey();
        check(animray::is_prime(16)).is_falsey();
        check(animray::is_prime(17)).is_truthy();
        check(animray::is_prime(18)).is_falsey();
        check(animray::is_prime(19)).is_truthy();
        check(animray::is_prime(20)).is_falsey();
    });


    auto const pg = suite.test("prime generator", [](auto check) {
        auto gen = animray::prime_generator<int>();
        check(gen()) == 2;
        check(gen()) == 3;
        check(gen()) == 5;
        check(gen()) == 7;
        check(gen()) == 11;
        check(gen()) == 13;
    });


    auto const pf = suite.test("prime factors", [](auto check) {
        std::vector<int> p10 = animray::prime_factors(10);
        check(p10.size()) == 2u;
        check(p10[0]) == 2;
        check(p10[1]) == 5;

        std::vector<int> p17 = animray::prime_factors(17);
        check(p17.size()) == 1u;
        check(p17[0]) == 17;

        std::vector<int> p600 = animray::prime_factors(600);
        check(p600.size()) == 6u;
        check(p600[0]) == 2;
        check(p600[1]) == 2;
        check(p600[2]) == 2;
        check(p600[3]) == 3;
        check(p600[4]) == 5;
        check(p600[5]) == 5;
    });


}
