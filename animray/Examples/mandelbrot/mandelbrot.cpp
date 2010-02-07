/*
    Copyright 2010, Kirit Saelensminde.
    http://www.kirit.com/AnimRay

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
#include <animray/mandelbrot.hpp>


FSL_MAIN(
    L"mandelbrot",
    L"Mandelbrot, Copyright 2010 Kirit Saelensminde"
)( fostlib::ostream &out, fostlib::arguments &args ) {
    boost::filesystem::wpath output_filename =
        fostlib::coerce< boost::filesystem::wpath >(args[1].value("out.tga"))
    ;
    int width = fostlib::coerce< int >( args[2].value("100") );
    int height = fostlib::coerce< int >( args[3].value("100") );
    out << "Creating image " << output_filename
        <<", size " << width << " x " << height << std::endl
    ;

    typedef animray::film< uint8_t > film_type;
    film_type output(width, height);

    typedef double precision;
    precision centre_x = fostlib::coerce< precision >(
        args.commandSwitch("cx").value("0")
    );
    precision centre_y = fostlib::coerce< precision >(
        args.commandSwitch("cy").value("0")
    );
    precision radius = fostlib::coerce< precision >(
        args.commandSwitch("r").value("2")
    );
    std::size_t bits = fostlib::coerce< int >(
        args.commandSwitch("bits").value("8")
    );

    out << "Centre image at " << centre_x << ", " << centre_y <<
        " with radius of " << radius <<
        " to " << bits << " bits" <<
    std::endl;

    output.transform(
        animray::mandelbrot::iterations< film_type, precision >(
            width, height, centre_x, centre_y, radius, bits
        )
    );

    animray::targa(output_filename, output);

    return 0;
}
