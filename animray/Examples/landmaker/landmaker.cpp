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
#include <animray/color/hls.hpp>
#include <boost/random.hpp>
#include <boost/math/constants/constants.hpp>


namespace {


    const double pi = boost::math::constants::pi<double>();


    template<typename V>
    V square(V v) {
        return v * v;
    }


    struct circle {
        float cx, cy, r;
        bool contains(std::size_t x, std::size_t y)  const {
            return square(cx - x) + square(cy - y) < square(r);
        }
    };


    void more_circles(boost::mt19937 &rng, const ::circle &within,
            std::vector< ::circle > &circles) {
        boost::uniform_real<float>
            r_radians(0, 2 * pi), r_distance(0, within.r);
        boost::variate_generator<
            boost::mt19937&, boost::uniform_real<float> >
                radians(rng, r_radians), distance(rng, r_distance);
        float theta{radians()}, length{distance()};
        for ( auto i = 0; i < 3; ++i ) {
            circle next{
                within.cx + length * std::cos(theta),
                within.cy + length * std::sin(theta),
                within.r / 2.f};
            circles.push_back(next);
            if ( within.r > 2.f ) {
                more_circles(rng, next, circles);
            }
        }
    }


}


FSL_MAIN(
    L"landmaker",
    L"LandMaker, Copyright 2010-2014 Kirit Saelensminde"
)( fostlib::ostream &out, fostlib::arguments &args ) {
    boost::filesystem::wpath output_filename =
        fostlib::coerce< boost::filesystem::wpath >(args[1].value("out.tga"));
    int width = fostlib::coerce< int >( args[2].value("100") );
    int height = fostlib::coerce< int >( args[3].value("100") );

    boost::mt19937 rng(static_cast<unsigned int>(std::time(0)));
    std::vector< ::circle > circles;
    circle start{width  / 2.f, height / 2.f, std::min(width, height) / 4.f};
    for ( auto i = 0; i != 3; ++i ) {
        circles.push_back(start);
        more_circles(rng, start, circles);
    }

    out << "Creating image " << output_filename
        <<", size " << width << " x " << height
        << " using " << circles.size() << " circles" << std::endl;

    typedef animray::film< animray::rgb< uint8_t > > film_type;
    film_type output(width, height,
        [&circles](film_type::size_type x, film_type::size_type y) {
            double weight = 0.0025 * std::count_if(circles.begin(), circles.end(),
                [=](const circle &c) -> bool {
                    return c.contains(x, y);
                });
            animray::hls<double> h(int(360.0 * weight) % 360, 0.5, 1.0);
            animray::rgb<double> c(
                fostlib::coerce< animray::rgb<double> >(h));
            return animray::rgb<uint8_t>(
                c.red() * 255, c.green() * 255, c.blue() * 255);
        });

    animray::targa(output_filename, output);

    return 0;
}

