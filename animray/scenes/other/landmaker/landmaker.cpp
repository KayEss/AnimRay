/**
    Copyright 2010-2019, [Kirit Saelensminde](https://kirit.com/AnimRay)

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


#include <fost/unicode>
#include <fost/main>
#include <fost/coerce/ints.hpp>
#include <animray/targa.hpp>
#include <animray/color/hls.hpp>
#include <animray/threading/random-generator.hpp>
#include <animray/maths/angles.hpp>


namespace {


    template<typename V>
    V square(V v) {
        return v * v;
    }


    struct circle {
        float cx, cy, r;
        bool contains(std::size_t x, std::size_t y) const {
            return square(cx - x) + square(cy - y) < square(r);
        }
    };


    void more_circles(
            animray::random::engine<> &rng,
            const ::circle &within,
            std::vector<::circle> &circles) {
        std::uniform_real_distribution<float> radians(0, 2 * animray::pi),
                distance(0, within.r);
        float theta{radians(rng.e)}, length{distance(rng.e)};
        for (auto i = 0; i < 3; ++i) {
            circle next{
                    within.cx + length * std::cos(theta),
                    within.cy + length * std::sin(theta), within.r / 2.f};
            circles.push_back(next);
            if (within.r > 2.f) { more_circles(rng, next, circles); }
        }
    }


}


FSL_MAIN("landmaker", "LandMaker, Copyright 2010-2019 Kirit Saelensminde")
(fostlib::ostream &out, fostlib::arguments &args) {
    fostlib::fs::path output_filename =
            fostlib::coerce<fostlib::fs::path>(args[1].value_or("out.tga"));
    int width = fostlib::coerce<int>(args[2].value_or("100"));
    int height = fostlib::coerce<int>(args[3].value_or("100"));

    animray::random::engine<> rng;
    std::vector<::circle> circles;
    circle start{width / 2.f, height / 2.f, std::min(width, height) / 4.f};
    for (auto i = 0; i != 3; ++i) {
        circles.push_back(start);
        more_circles(rng, start, circles);
    }

    out << "Creating image " << output_filename << ", size " << width << " x "
        << height << " using " << circles.size() << " circles" << std::endl;

    double const scale = std::sqrt(0.05 * width * height) / circles.size();
    using film_type = animray::film<animray::rgb<uint8_t>>;
    auto output = film_type(
            width, height,
            [&circles, scale](film_type::size_type x, film_type::size_type y) {
                double const weight = scale
                        * std::count_if(circles.begin(), circles.end(),
                                        [=](const circle &c) -> bool {
                                            return c.contains(x, y);
                                        });
                animray::hls<double> h(int(360.0 * weight) % 360, 0.5, 1.0);
                auto const c = fostlib::coerce<animray::rgb<double>>(h);
                return animray::rgb<uint8_t>(
                        c.red() * 255, c.green() * 255, c.blue() * 255);
            });

    animray::targa(output_filename, output);

    return 0;
}
