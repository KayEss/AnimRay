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


#include <animray/color/concept.hpp>
#include <animray/color/hsl.hpp>
#include <animray/test.hpp>
#include <felspar/test.hpp>


namespace {


    auto const suite = felspar::testsuite(__FILE__);


    static_assert(animray::Spectrum<animray::hsl<float>>);


    void hsl_is_rgb(
            auto check, float h, float s, float l, float r, float g, float b) {
        animray::hsl<float> f(h, s, l);
        auto const t(animray::convert_to<animray::rgb<float>>(f));
        animray::check_close(check, t.array()[0], r);
        animray::check_close(check, t.array()[1], g);
        animray::check_close(check, t.array()[2], b);
    }
    auto const rgb = suite.test("to_rgb", [](auto check) {
        // Test data taken from http://en.wikipedia.org/wiki/HSL_and_HSV
        hsl_is_rgb(check, 0, 0, 0, 0, 0, 0); // black
        hsl_is_rgb(check, 0, 0, 1, 1, 1, 1); // white
        hsl_is_rgb(check, 0, 0, .5f, .5f, .5f, .5f); // gray
        hsl_is_rgb(check, 0, 1, .5f, 1, 0, 0); // red
        hsl_is_rgb(check, 60, 1, .375f, .75f, .75f, 0); // olive
        hsl_is_rgb(check, 49.5f, .893f, .497f, .941f, .785f, .053f); // orange
        hsl_is_rgb(
                check, 240.5f, .290f, .607f, .495f, .493f,
                .721f); // light purple
    });


}
