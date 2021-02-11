/**
    Copyright 2010-2021, [Kirit Saelensminde](https://kirit.com/AnimRay)

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


#include <animray/color/hsl.hpp>
#include <animray/formats/targa.hpp>
#include <animray/maths/angles.hpp>
#include <animray/threading/random-generator.hpp>
#include <fost/main>
#include <fost/unicode>


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
            std::vector<::circle> &circles,
            std::size_t const splits) {
        std::uniform_real_distribution<float> radians(0, 2 * animray::pi),
                distance(0, within.r);
        float theta{radians(rng.e)}, length{distance(rng.e)};
        for (std::size_t i{}; i != splits; ++i) {
            circle const next{
                    within.cx + length * std::cos(theta),
                    within.cy + length * std::sin(theta), within.r / 2.f};
            circles.push_back(next);
            if (within.r > 2.f) { more_circles(rng, next, circles, splits); }
        }
    }


}


FSL_MAIN("landmaker", "LandMaker, Copyright 2010-2021 Kirit Saelensminde")
(fostlib::ostream &out, fostlib::arguments &args) {
    auto const output_filename =
            fostlib::coerce<std::filesystem::path>(args[1].value_or("out.tga"));
    std::size_t const width = fostlib::coerce<int>(args[2].value_or("100"));
    std::size_t const height = fostlib::coerce<int>(args[3].value_or("100"));

    auto const ccount =
            fostlib::coerce<std::size_t>(args.commandSwitch("c").value_or("3"));
    auto const splits =
            fostlib::coerce<std::size_t>(args.commandSwitch("s").value_or("3"));
    auto const mag =
            fostlib::coerce<float>(args.commandSwitch("m").value_or("1"));
    auto const radius =
            fostlib::coerce<std::optional<float>>(args.commandSwitch("r"))
                    .value_or(std::min(width, height) / 4.f);

    auto const format =
            fostlib::coerce<std::size_t>(args.commandSwitch("f").value_or("0"));


    out << "Circle count " << ccount << ", radius " << radius << ", with split "
        << splits << " and elevation magnification " << mag << std::endl;

    animray::random::engine<> rng;
    std::vector<::circle> circles;
    circle start{width / 2.f, height / 2.f, radius};
    circles.push_back(start);
    for (std::size_t i{}; i != ccount; ++i) {
        more_circles(rng, start, circles, splits);
    }

    out << "Creating image " << output_filename << ", size " << width << " x "
        << height << " using " << circles.size() << " circles" << std::endl;

    float const scale = mag * std::sqrt(0.05 * width * height) / circles.size();
    animray::film<float> const heights{
            width, height, [&circles, scale](auto const x, auto const y) {
                auto const count = std::count_if(
                        circles.begin(), circles.end(),
                        [=](const circle &c) -> bool {
                            return c.contains(x, y);
                        });
                return std::clamp(scale * count, 0.0f, 1.0f);
            }};

    switch (format) {
    case 0:
        animray::targa(
                output_filename,
                animray::film<animray::rgb<uint8_t>>{
                        width, height, [&heights](auto const x, auto const y) {
                            animray::hsl<float> h(
                                    300.0f * heights[x][y], 1.0f, 0.5f);
                            auto const c =
                                    fostlib::coerce<animray::rgb<float>>(h);
                            return animray::rgb<uint8_t>(
                                    c.red() * 255, c.green() * 255,
                                    c.blue() * 255);
                        }});
        break;
    case 1:
        animray::targa(
                output_filename,
                animray::film<animray::luma<uint8_t>>{
                        width, height, [&heights](auto const x, auto const y) {
                            return animray::luma<uint8_t>(heights[x][y] * 255);
                        }});
        break;
    default:
        std::cerr << "Unknown format number " << format << "options are:\n";
        std::cerr << "0 -- False colour HSL\n";
        std::cerr << "1 -- Grayscale matte\n";
    }

    return 0;
}
