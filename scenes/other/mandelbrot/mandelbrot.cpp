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


#include <animray/color/hsl.hpp>
#include <animray/formats/targa.hpp>
#include <animray/mandelbrot.hpp>
#include <fost/main>
#include <fost/unicode>


FSL_MAIN("mandelbrot", "Mandelbrot, Copyright 2010-2021 Kirit Saelensminde")
(fostlib::ostream &out, fostlib::arguments &args) {
    auto const output_filename =
            fostlib::coerce<std::filesystem::path>(args[1].value_or("out.tga"));
    int width = fostlib::coerce<int>(args[2].value_or("100"));
    int height = fostlib::coerce<int>(args[3].value_or("100"));
    out << "Creating image " << output_filename << ", size " << width << " x "
        << height << std::endl;

    typedef double precision;
    precision centre_x =
            fostlib::coerce<precision>(args.commandSwitch("cx").value_or("0"));
    precision centre_y =
            fostlib::coerce<precision>(args.commandSwitch("cy").value_or("0"));
    precision diameter =
            fostlib::coerce<precision>(args.commandSwitch("d").value_or("2"));
    std::size_t bits =
            fostlib::coerce<int>(args.commandSwitch("bits").value_or("8"));
    double hue =
            fostlib::coerce<double>(args.commandSwitch("h").value_or("0.0"));

    out << "Centre image at " << centre_x << ", " << centre_y
        << " with diameter of " << diameter << " to " << bits << " bits"
        << std::endl;

    typedef animray::film<animray::rgb<uint8_t>> film_type;
    film_type output(
            width, height,
            animray::mandelbrot::transformer<film_type, precision>(
                    width, height, centre_x, centre_y, diameter, bits,
                    [hue](unsigned int d, std::size_t b) {
                        if (d) {
                            unsigned int m = (1u << b) - 1u;
                            animray::hsl<double> h(
                                    int(hue + 360.0 * d / m) % 360, 1.0, 0.5);
                            animray::rgb<double> c(
                                    fostlib::coerce<animray::rgb<double>>(h));
                            return animray::rgb<uint8_t>(
                                    c.red() * 255, c.green() * 255,
                                    c.blue() * 255);
                        } else {
                            return animray::rgb<uint8_t>();
                        }
                    }));

    animray::targa(output_filename, output);

    return 0;
}
