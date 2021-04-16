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


#include <animray/cli/main.hpp>
#include <animray/color/hsl.hpp>
#include <animray/formats/targa.hpp>
#include <animray/maths/angles.hpp>
#include <animray/threading/random-generator.hpp>
#include <iostream>


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


int main(int argc, char const *const argv[]) {
    auto const args =
            animray::cli::arguments{argc, argv, "landmaker.tga", 150, 100};

    auto const ccount = args.switch_value('c', 3u);
    auto const splits = args.switch_value('s', 3);
    auto const mag = args.switch_value('m', 1);
    auto const radius = args.switch_value('r', 4.0f);
    auto const format = args.switch_value('F', 0);

    std::cout << "Circle count " << ccount << ", radius " << radius
              << ", with split " << splits << " and elevation magnification "
              << mag << std::endl;

    animray::random::engine<> rng;
    std::vector<::circle> circles;
    circle start{args.width / 2.f, args.height / 2.f, radius};
    circles.push_back(start);
    for (std::size_t i{}; i != ccount; ++i) {
        more_circles(rng, start, circles, splits);
    }

    std::cout << "Creating image " << args.output_filename << ", size "
              << args.width << " x " << args.height << " using "
              << circles.size() << " circles" << std::endl;

    float const scale =
            mag * std::sqrt(0.05 * args.width * args.height) / circles.size();
    animray::film<float> const heights{
            args.width, args.height,
            [&circles, scale](auto const x, auto const y) {
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
                args.output_filename,
                animray::film<animray::rgb<uint8_t>>{
                        args.width, args.height,
                        [&heights](auto const x, auto const y) {
                            animray::hsl<float> h(
                                    300.0f * heights[x][y], 1.0f, 0.5f);
                            auto const c =
                                    animray::convert_to<animray::rgb<float>>(h);
                            return animray::rgb<uint8_t>(
                                    c.red() * 255, c.green() * 255,
                                    c.blue() * 255);
                        }});
        break;
    case 1:
        animray::targa(
                args.output_filename,
                animray::film<animray::luma<uint8_t>>{
                        args.width, args.height,
                        [&heights](auto const x, auto const y) {
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
