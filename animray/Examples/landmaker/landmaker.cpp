/*
    Copyright 2010-2014, Kirit Saelensminde.
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
#include <animray/hls.hpp>
#include <boost/random.hpp>


namespace {


    struct circle {
        std::size_t cx, cy, r2;
    };


    void create(boost::mt19937 &rng) {
        boost::variate_generator<
            boost::mt19937&,
            boost::uniform_int<>
        > rx( rng, 100 ), ry( rng, 100 );
    }

    //     boost::uniform_int<>
    //         uix(size.lower_left().x(), size.top_right().x()),
    //         uiy(size.lower_left().y(), size.top_right().y())
    //     ;
}


FSL_MAIN(
    L"landmaker",
    L"LandMaker, Copyright 2010-2014 Kirit Saelensminde"
)( fostlib::ostream &out, fostlib::arguments &args ) {
    boost::filesystem::wpath output_filename =
        fostlib::coerce< boost::filesystem::wpath >(args[1].value("out.tga"));
    int width = fostlib::coerce< int >( args[2].value("100") );
    int height = fostlib::coerce< int >( args[3].value("100") );
    out << "Creating image " << output_filename
        <<", size " << width << " x " << height << std::endl;

    boost::mt19937 rng(static_cast<unsigned int>(std::time(0)));
    std::vector< ::circle > circles;

    typedef animray::film< animray::rgb< uint8_t > > film_type;
    film_type output(width, height,
        [](film_type::size_type x, film_type::size_type y) {
            double weight = 0.1;
            animray::hls<double> h(int(360.0 * weight) % 360, 0.5, 1.0);
            animray::rgb<double> c(
                fostlib::coerce< animray::rgb<double> >(h));
            return animray::rgb<uint8_t>(
                c.red() * 255, c.green() * 255, c.blue() * 255);
        });

    animray::targa(output_filename, output);

    return 0;
}

