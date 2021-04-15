/*
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


#include <animray/cli/main.hpp>
#include <animray/color/hsl.hpp>
#include <animray/formats/targa.hpp>
#include <animray/mandelbrot.hpp>
#include <iostream>


int main(int argc, char const *const argv[]) {
    auto const args =
            animray::cli::arguments{argc, argv, "mandelbrot.tga", 100, 100};

    std::cout << "Creating image " << args.output_filename << ", size "
              << args.width << " x " << args.height << std::endl;

    using precision = long double;
    auto const centre_x = args.switch_value('x', 0.0L);
    auto const centre_y = args.switch_value('y', 0.0L);
    auto const diameter = args.switch_value('d', 2.0L);
    std::size_t const bits = args.switch_value('b', 8);
    double const hue = args.switch_value('H', 0.0);

    std::cout << "Centre image at " << centre_x << ", " << centre_y
              << " with diameter of " << diameter << " to " << bits << " bits"
              << std::endl;

    using film_type = animray::film<animray::rgb<uint8_t>>;
    film_type output(
            args.width, args.height,
            animray::mandelbrot::transformer<film_type, precision>(
                    args.width, args.height, centre_x, centre_y, diameter, bits,
                    [hue](unsigned int d, std::size_t b) {
                        if (d) {
                            unsigned int m = (1u << b) - 1u;
                            animray::hsl<double> h(
                                    int(hue + 360.0 * d / m) % 360, 1.0, 0.5);
                            animray::rgb<double> c(
                                    animray::convert_to<animray::rgb<double>>(
                                            h));
                            return animray::rgb<uint8_t>(
                                    c.red() * 255, c.green() * 255,
                                    c.blue() * 255);
                        } else {
                            return animray::rgb<uint8_t>();
                        }
                    }));

    animray::targa(args.output_filename, output);

    return 0;
}
